#pragma once
#include "Advancement.h"

class AdvancementTreeNode
{
public:
  AdvancementTreeNode(Advancement* advancementIn, AdvancementTreeNode* parentIn, AdvancementTreeNode* siblingIn, uint64_t indexIn, uint32_t xIn);
  static void layout(Advancement* root);

private:
  AdvancementTreeNode* buildSubTree(Advancement* advancementIn, AdvancementTreeNode* previous);
  void firstWalk();
  float secondWalk(float p_192319_1_, int p_192319_2_, float p_192319_3_);
  void thirdWalk(float yIn);
  void executeShifts();
  AdvancementTreeNode* getFirstChild();
  AdvancementTreeNode* getLastChild();
  AdvancementTreeNode* apportion(AdvancementTreeNode* nodeIn);
  void moveSubtree(AdvancementTreeNode* nodeIn, float p_192316_2_);
  AdvancementTreeNode* getAncestor(AdvancementTreeNode* p_192326_1_, AdvancementTreeNode* p_192326_2_);
  void updatePosition();

   Advancement* advancement;
   AdvancementTreeNode* parent;
   AdvancementTreeNode* sibling;
   uint64_t index;
   std::list<> children;
   AdvancementTreeNode* ancestor;
   AdvancementTreeNode* thread;
   uint32_t x;
   float y;
   float mod;
   float change;
   float shift;
};
