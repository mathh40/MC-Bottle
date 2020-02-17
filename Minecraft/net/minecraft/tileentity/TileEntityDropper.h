#include "TileEntityDispenser.h"

class TileEntityDropper :public TileEntityDispenser
{
public:
	static void registerFixesDropper(DataFixer fixer);
	std::string getName() const override;
	std::string getGuiID() const override;
private:
};