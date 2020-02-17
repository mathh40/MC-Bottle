#include <dynamic_bitset.hpp>

class IRecipe;
class RecipeBook
{
public:
	void copyFrom(RecipeBook that);
	void unlock(IRecipe* recipe);
	bool isUnlocked(IRecipe* recipe);
	void lock(IRecipe* recipe);
	bool isNew(IRecipe* recipe);
	void markSeen(IRecipe* recipe);
	void markNew(IRecipe* recipe);
	bool isGuiOpen() const;
	void setGuiOpen(bool open);
	bool isFilteringCraftable() const;
	void setFilteringCraftable(bool shouldFilter) const;
	
protected:
	int32_t getRecipeId(IRecipe* recipe);
	
	dynamic_bitset<> recipes;
	dynamic_bitset<> newRecipes;
	bool bisGuiOpen;
	bool bisFilteringCraftable;
private:
};
