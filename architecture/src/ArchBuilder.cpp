//
// Created by wilhelma on 1/10/17.
//

#include "../include/ArchBuilder.h"

#include <json11.hpp>
#include <unordered_map>
#include <algorithm>
#include <iostream>

#include "../include/ArchRule.h"

using json11::Json;

namespace pcv {

void removeDuplicates(Artifact_t &as, Artifact_t &newArtifact, Artifact_t *root = nullptr)
{
  static std::unordered_map<const SoftwareEntity *, Artifact_t *> addedEntities;

  if (!root) root = &newArtifact;

  auto getCommonAncestor = [&newArtifact, &root](Artifact_t *ls, Artifact_t *rs) {
    auto getArchVector = [](Artifact_t *cs) {
      std::vector<Artifact_t *> res{cs};
      while (res.front()->parent && res.front()->parent->parent)
        res.insert(begin(res), res.front()->parent);
      return res;
    };

    auto buildArtifactName = [](const Artifact_t &lhs, const Artifact_t &rhs) {
      return lhs.name + DELIM + rhs.name;
    };

    auto getArtifact = [](const std::string &name,
                          const std::vector<std::unique_ptr<Artifact_t>> &v) {
      Artifact_t *s{nullptr};
      auto it = std::find_if(
          begin(v),
          end(v),
          [&name](const std::unique_ptr<Artifact_t> &s) { return s->name == name; }
      );
      if (it != end(v)) s = it->get();

      return s;
    };

    auto appendArtifact = [](const std::string &name, Artifact_t *parent) {
      parent->children.emplace_back(std::unique_ptr<Artifact_t>{new Artifact_t(name, parent)});
      return parent->children.back().get();
    };

    /* begin of getCommonAncestor() */
    auto lhs = getArchVector(ls);
    auto rhs = getArchVector(rs);

    // prepare base artifact
    auto tSet = lhs[0];
    if (tSet != rhs[0]) {
      std::string setName = buildArtifactName(*rhs[0], *tSet);
      tSet = getArtifact(setName, root->children);
      if (!tSet) tSet = appendArtifact(setName, root);
    }

    // prepare sub-artifacts
    for (uint32_t i = 1; i < std::min(lhs.size(), rhs.size()); ++i) {
      if (lhs[i]->name != rhs[i]->name) break;

      auto tmp = getArtifact(lhs[i]->name, tSet->children);
      if (!tmp) tmp = appendArtifact(lhs[i]->name, tSet);
      tSet = tmp;
    }

    return tSet;
  };

  for (auto i = begin(as.entities); i != end(as.entities);) {
    auto s = addedEntities.find(*i);
    if (s == end(addedEntities)) {
      addedEntities[*i] = &as;
      ++i;
    } else {
      auto commonSet = getCommonAncestor(&as, s->second);
      commonSet->entities.insert(*i);
      addedEntities[*i]->entities.erase(s->first);
      addedEntities[*i] = commonSet;
      i = as.entities.erase(i);
    }
  }

  for (auto &child : as.children)
     removeDuplicates(*child.get(), newArtifact, root);
}

void removeDuplicatedsAndAppend(Artifact_t &as)
{
  Artifact_t newArtifact("newArtifact", nullptr);
  removeDuplicates(as, newArtifact);

  // relink parent of the new root artifacts
  std::for_each(begin(newArtifact.children),
                end(newArtifact.children),
                [&as](const std::unique_ptr<Artifact_t> &ptr) { ptr->parent = &as; }
  );

  // move new root artifacts to root
  as.children.insert(
      end(as.children),
      std::make_move_iterator(begin(newArtifact.children)),
      std::make_move_iterator(end(newArtifact.children))
  );
}

void removeEmptyArtifacts(Artifact_t &as)
{
  for (auto i = begin(as.children); i != end(as.children);) {
    removeEmptyArtifacts(*i->get());
    if ((*i)->entities.empty() && (*i)->children.empty())
      i = as.children.erase(i);
    else
      ++i;
  }
}

void ArchBuilder::apply(ArchRule *rule) noexcept
{
  rule->apply(artifact_, ctxt_);
}

void addArtifact(Json::array *entities,
                 const Artifact_t *set,
                 const std::string &artifactName = "")
{
  for (auto &entity : set->entities) {
    entities->emplace_back(
        Json::object {
            {"id", entity->name},
            {"Id", static_cast<int>(entity->id)},
            {"Type", static_cast<int>(entity->getEntityType())},
            {"Name", entity->name},
            {"Image", entity->img->name},
            {"Namespace", entity->nmsp->name},
            {"File", entity->file},
            {"Line", static_cast<int>(entity->line)},
        }
    );
  }

  for (auto &child : set->children) {
    auto childObjects = Json::array {};
    addArtifact(&childObjects, child.get(), child->name);

    entities->emplace_back(
        Json::object {
            {"id", child->name},
            {"children", childObjects},
        }
    );
  }
}

void addLinks(Json::array *links, const Artifact_t *set)
{
  auto isEmpty = [&set](const std::string &aName) {
    auto it = std::find_if(
        begin(set->children),
        end(set->children),
        [&aName](const std::unique_ptr<Artifact_t> &a) { return a->name == aName; } );
    return it == end(set->children);
  };

  auto addLink = [&links](const std::string &lhs, const std::string &rhs) {
    links->emplace_back(
        Json::object {
            {"source", lhs},
            {"target", rhs},
            {"value", 5},
        }
    );
  };

  for (auto &child : set->children) {
    std::vector<std::string> artifactIds;

    // first (maybe only) artifact
    auto pos = child->name.find(DELIM);
    auto tmp = child->name.substr(0, pos);
    if (!isEmpty(tmp))
      artifactIds.emplace_back( tmp );

    if (pos != std::string::npos) {
      for (; pos++ != std::string::npos; pos = child->name.find(DELIM, pos + 1)) {
        auto tmp = child->name.substr( pos, child->name.find(DELIM, pos) );
        if (!isEmpty(tmp))
          artifactIds.emplace_back( tmp );
      }

      for (auto &id : artifactIds)
        addLink(id, child->name);
    }
  }
}

std::ostream &operator<<(std::ostream &os, const ArchBuilder &obj)
{
  Json::array childs{};
  addArtifact(&childs, &obj.artifact_);

  Json::array links{};
  addLinks(&links, &obj.artifact_);

  auto sets = Json::object {
      {"id", "root"},
      {"children", childs},
      {"links", links},
  };

  os << Json(sets).dump();
  return os;
}

void ArchBuilder::finish()
{
  removeDuplicatedsAndAppend(artifact_);
  removeEmptyArtifacts(artifact_);
}

}  // namespace pcv
