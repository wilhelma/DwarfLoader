//
// Created by wilhelma on 1/20/17.
//

#include "ArchBuilder.h"

using pcv::dwarf::Artifact_t;
using pcv::dwarf::SoftwareEntity;

void fillArtifact(Artifact_t &artifact, const std::array<SoftwareEntity*, 2>& entities)
{
  artifact.name = "default";

  // {a, [], [{b, [], [{c, [], [{d, [entityA], []}] }] }] }
  artifact.children.emplace_back( std::unique_ptr<Artifact_t>{ new Artifact_t("a", &artifact) } );
  auto a = artifact.children.back().get();
  a->children.emplace_back( std::unique_ptr<Artifact_t>{ new Artifact_t("b", a) } );
  auto b = a->children.back().get();
  b->children.emplace_back( std::unique_ptr<Artifact_t>{ new Artifact_t("c", b) } );
  auto c = b->children.back().get();
  c->children.emplace_back( std::unique_ptr<Artifact_t>{ new Artifact_t("d", c) } );
  auto d = c->children.back().get();
  d->entities.insert( entities[0] );

  // {a, [], [{b, [], [{e, [entityA, entityB], []}] }] }] }
  b->children.emplace_back( std::unique_ptr<Artifact_t>{ new Artifact_t("e", b) } );
  auto e = b->children.back().get();
  e->entities.insert( entities[0] );
  e->entities.insert( entities[1] );

  // {x, [], [{b, [], [{c, [], [{f, [entityB], []}] }] }] }
  artifact.children.emplace_back( std::unique_ptr<Artifact_t>{ new Artifact_t("x", &artifact) } );
  auto x = artifact.children.back().get();
  x->children.emplace_back( std::unique_ptr<Artifact_t>{ new Artifact_t("b", x) } );
  auto b2 = x->children.back().get();
  b2->children.emplace_back( std::unique_ptr<Artifact_t>{ new Artifact_t("c", b2) } );
  auto c2 = b2->children.back().get();
  c2->children.emplace_back( std::unique_ptr<Artifact_t>{ new Artifact_t("f", c2) } );
  auto f = c2->children.back().get();
  f->entities.insert( entities[1] );
}
