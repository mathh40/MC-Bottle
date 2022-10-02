#include "BlockStateContainer.h"
#include <MapPopulator.h>
#include <math/Cartesian.h>
#include <properties/IProperty.h>
#include <regex>


static auto NAME_PATTERN = std::regex( "^[a-z0-9_]+$");
static constexpr auto GET_NAME_FUNC = [](IProperty *p_apply_1_) {
  return p_apply_1_ == nullptr ? "<NULL>" : p_apply_1_->getName();
};

BlockStateContainer::BlockStateContainer(Block *blockIn, std::span<IProperty *> propertiesIn) : block(blockIn)
{ 
  std::unordered_map<std::string, IProperty *> tempmap;

  for (auto iproperty : propertiesIn) {
    validateProperty(blockIn, iproperty);
    tempmap.emplace(iproperty->getName(), iproperty);
  }

  properties = tempmap;
  std::unordered_map<std::unordered_map<IProperty *, std::set<std::any>>, IBlockState *> map2;
  std::vector<std::shared_ptr<BlockStateContainer::StateImplementation>> list1;
  auto var12 = Cartesian::cartesianProduct(getAllowedValues());

  for (auto list : var12) {
    auto map1 = MapPopulator::createMap(properties, list);
    std::shared_ptr<BlockStateContainer::StateImplementation> blockstatecontainer$stateimplementation =
      std::make_shared<BlockStateContainer::StateImplementation>(blockIn, map1);
    map2.emplace(map1, blockstatecontainer$stateimplementation.get());
    list1.emplace_back(blockstatecontainer$stateimplementation);
  }

  for (auto blockstatecontainer$stateimplementation1 : list1) {
    blockstatecontainer$stateimplementation1->buildPropertyValueTable(map2);
  }

  validStates = list1;
}

std::string BlockStateContainer::validateProperty(Block *block, IProperty *propertie)
{
  auto s = propertie->getName();
  if (!std::regex_match(s, NAME_PATTERN)) {
    throw std::runtime_error("Block: " + block->getClass() + " has invalidly named property: " + s);
  } else {
    auto var3 = propertie->getAllowedValues().begin();
    std::string s1;
    do {
      if (var3 == propertie->getAllowedValues().end()) { return s; }
      s1 = propertie->getName(*var3);
    } while (std::regex_match(s1, NAME_PATTERN));

    throw std::runtime_error(
      "Block: " + block->getClass() + " has property: " + s + " with invalidly named value: " + s1);
  }
}

std::vector<std::shared_ptr<BlockStateContainer::StateImplementation>> BlockStateContainer::getValidStates() {
  return validStates;
}

std::vector<std::set<std::any>> BlockStateContainer::getAllowedValues()
{
  std::vector<std::set<std::any>> list;

  for (auto &iproperty : properties) { list.emplace_back(iproperty.second->getAllowedValues()); }

  return list;
}
