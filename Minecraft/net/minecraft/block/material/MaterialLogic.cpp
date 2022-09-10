#include"MaterialLogic.h"

 MaterialLogic::MaterialLogic(MapColor color) : Material(color) {
     setAdventureModeExempt();
 }

 bool MaterialLogic::isSolid() {
      return false;
 }

 bool MaterialLogic::blocksLight() {
      return false;
  }

 bool MaterialLogic::blocksMovement() {
     return false;
 }
