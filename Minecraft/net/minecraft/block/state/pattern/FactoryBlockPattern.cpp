#include "FactoryBlockPattern.h"

FactoryBlockPattern::FactoryBlockPattern() { symbolMap.emplace('.', true); }

FactoryBlockPattern FactoryBlockPattern::aisle(std::vector<std::string>& aisleIn)
{
  if (!aisleIn.empty() && !aisleIn[0].empty()) {
         if (depth.empty()) {
      aisleHeight = aisleIn.size();
            rowWidth = aisleIn[0].size();
         }

         if (aisleIn.size() != aisleHeight) {
           throw std::runtime_error(fmt::format("Expected aisle with height of {}, but was given one with a height of {})",aisleHeight,aisleIn.size()));
         } else {
           for (auto &s : aisleIn) {
               if (s.size() != rowWidth) {
               throw std::runtime_error(fmt::format("Not all rows in the given aisle are the correct width (expected {}, found one with {})",rowWidth,s.size()));
               }

               for (auto &c0 : s) {
                  if (!symbolMap.contains(c0)) {
                     symbolMap.emplace(c0,false);
                  }
               }
            }

            depth.emplace_back(aisleIn);
            return *this;
         }
      } else {
    throw std::runtime_error("Empty pattern for aisle");
      }
}

FactoryBlockPattern FactoryBlockPattern::start() {
    return FactoryBlockPattern();
}

BlockPattern FactoryBlockPattern::build() {
    return BlockPattern(makePredicateArray());
 }

multivec3<> &FactoryBlockPattern::makePredicateArray()
{
  checkMissingPredicates();
      multivec3<> predicate = (Predicate[][][])((Predicate[][][])((Predicate[][][])Array.newInstance(Predicate.class, new int[]{this.depth.size(), this.aisleHeight, this.rowWidth})));

      for(auto i = 0; i < depth.size(); ++i) {
        for (auto j = 0; j < aisleHeight; ++j) {
          for (auto k = 0; k < rowWidth; ++k) {
               predicate[i][j][k] = (Predicate)symbolMap.get(((String[])((String[])depth.get(i)))[j].charAt(k));
            }
         }
      }

      return predicate;
      }

void FactoryBlockPattern::checkMissingPredicates() const
{
          std::vector<const char> list;

      for (auto& entry: symbolMap){
         if (entry.second) {
            list.emplace_back(entry.first);
         }
      }

      if (!list.empty()) {
        throw std::logic_error("Predicates for character(s) " + Util::join(list,COMMA_JOIN) + " are missing");
      }
}
