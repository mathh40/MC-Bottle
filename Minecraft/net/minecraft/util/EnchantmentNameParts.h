#pragma once
#include "pcg_random.hpp"

#include <random>
#include <string>
#include <vector>

class EnchantmentNameParts
{
  public:
    static std::shared_ptr<EnchantmentNameParts> getInstance();
    std::string generateNewRandomName(FontRenderer fontRendererIn, int length);
    void reseedRandomGenerator(long seed);

  private:
    EnchantmentNameParts();
    static std::shared_ptr<EnchantmentNameParts> INSTANCE;
    pcg_extras::seed_seq_from<std::random_device> seed_source;
    pcg32 rand;
    std::array<std::string, 57> namePartsArray = {"the elder scrolls",
                                                  "klaatu",
                                                  "berata",
                                                  "niktu",
                                                  "xyzzy",
                                                  "bless",
                                                  "curse",
                                                  "light",
                                                  "darkness",
                                                  "fire",
                                                  "air",
                                                  "earth",
                                                  "water",
                                                  "hot",
                                                  "dry",
                                                  "cold",
                                                  "wet",
                                                  "ignite",
                                                  "snuff",
                                                  "embiggen",
                                                  "twist",
                                                  "shorten",
                                                  "stretch",
                                                  "fiddle",
                                                  "destroy",
                                                  "imbue",
                                                  "galvanize",
                                                  "enchant",
                                                  "free",
                                                  "limited",
                                                  "range of towards",
                                                  "inside",
                                                  "sphere",
                                                  "cube",
                                                  "self",
                                                  "other",
                                                  "ball",
                                                  "mental",
                                                  "physical",
                                                  "grow",
                                                  "shrink",
                                                  "demon",
                                                  "elemental",
                                                  "spirit",
                                                  "animal",
                                                  "creature",
                                                  "beast",
                                                  "humanoid",
                                                  "undead",
                                                  "fresh",
                                                  "stale",
                                                  "phnglui",
                                                  "mglwnafh",
                                                  "cthulhu",
                                                  "rlyeh",
                                                  "wgahnagl",
                                                  "fhtagnbaguette"};
};
