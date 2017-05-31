//
// Created by wilhelma on 1/20/17.
//

#include <gtest/gtest.h>

#include "../ArchBuilder.h"
#include "../../include/Context.h"
#include "./Utility.h"

using pcv::dwarf::ArchBuilder;
using pcv::dwarf::Context;
using pcv::dwarf::Class;
using pcv::dwarf::Image;
using pcv::dwarf::Namespace;
using pcv::dwarf::Artifact_t;

class ArchBuilderFixture : public ::testing::Test {
 protected:
  void SetUp() override
  {
    pCtxt_ = std::unique_ptr<Context>{ new Context() };
    pArchBuilder_ = std::unique_ptr<ArchBuilder>{ new ArchBuilder(*pCtxt_) };

  }

  void TearDown() override
  {

  }

  Artifact_t& getArtifact()
  {
    return pArchBuilder_->artifact_;
  }

  std::unique_ptr<Context> pCtxt_;
  std::unique_ptr<ArchBuilder> pArchBuilder_;
};

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

TEST_F(ArchBuilderFixture, Finish)
{
  Image img("img");
  Namespace nmsp("nmsp", nullptr);
  Class c1("Entity1", img, nmsp, nullptr, "file", 1, static_cast<Dwarf_Off>(1));
  Class c2("Entity2", img, nmsp, nullptr, "file", 1, static_cast<Dwarf_Off>(2));

  std::array<SoftwareEntity*, 2> arr { &c1, &c2 };

  auto &artifact = getArtifact();
  fillArtifact(artifact, arr);

  pArchBuilder_->finish();

  auto getArtifact = [](const std::vector<std::unique_ptr<Artifact_t>> &v,
                        const std::string &name) {
    return std::find_if(
        begin(v),
        end(v),
        [&name]( std::unique_ptr<Artifact_t> const &a) { return (a->name == name); }
    );
  };

  auto a = getArtifact(artifact.children, "a");
  ASSERT_NE(a, end(artifact.children));
  EXPECT_TRUE((*a)->entities.empty());
  EXPECT_EQ((*a)->children.size(), 1);

  auto ab = getArtifact((*a)->children, "b");
  ASSERT_NE(ab, end((*a)->children));
  EXPECT_TRUE((*ab)->children.empty());
  ASSERT_EQ((*ab)->entities.size(), 1);
  for (auto entity : (*ab)->entities)
    EXPECT_STREQ(entity->name.c_str(), "Entity1");
  EXPECT_TRUE((*ab)->children.empty());

  auto ax = getArtifact(artifact.children, "a/x");
  ASSERT_NE(ax, end(artifact.children));
  EXPECT_EQ((*ax)->children.size(), 1);
  EXPECT_TRUE((*ax)->entities.empty());

  auto axb = getArtifact((*ax)->children, "b");
  ASSERT_NE(axb, end((*ax)->children));
  ASSERT_EQ((*axb)->entities.size(), 1);
  for (auto entity : (*axb)->entities)
    EXPECT_STREQ(entity->name.c_str(), "Entity2");
  EXPECT_TRUE((*axb)->children.empty());
}
