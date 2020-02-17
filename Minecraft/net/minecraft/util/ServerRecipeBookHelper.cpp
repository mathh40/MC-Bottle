#include "ServerRecipeBookHelper.h"

void ServerRecipeBookHelper::func_194327_a(EntityPlayerMP p_194327_1_, std::optional<IRecipe> p_194327_2_, bool p_194327_3_)
{
	if (p_194327_2_.has_value && p_194327_1_.getRecipeBook().isUnlocked(p_194327_2_)) {
		field_194332_c = p_194327_1_;
		field_194333_d = p_194327_2_;
		field_194334_e = p_194327_3_;
		field_194337_h = p_194327_1_.openContainer.inventorySlots;
		Container container = p_194327_1_.openContainer;
		field_194335_f = null;
		field_194336_g = null;
		if (container instanceof ContainerWorkbench) {
			field_194335_f = ((ContainerWorkbench)container).craftResult;
			field_194336_g = ((ContainerWorkbench)container).craftMatrix;
		}
		else if (container instanceof ContainerPlayer) {
			field_194335_f = ((ContainerPlayer)container).craftResult;
			field_194336_g = ((ContainerPlayer)container).craftMatrix;
		}

		if (field_194335_f != null && field_194336_g != null && (func_194328_c() || p_194327_1_.isCreative())) {
			field_194331_b.clear();
			p_194327_1_.inventory.fillStackedContents(field_194331_b, false);
			field_194336_g.fillStackedContents(field_194331_b);
			if (field_194331_b.canCraft(p_194327_2_, (IntList)null)) {
				func_194329_b();
			}
			else {
				func_194326_a();
				p_194327_1_.connection.sendPacket(new SPacketPlaceGhostRecipe(p_194327_1_.openContainer.windowId, p_194327_2_));
			}

			p_194327_1_.inventory.markDirty();
		}
	}
}

void ServerRecipeBookHelper::func_194326_a() {
	InventoryPlayer inventoryplayer = field_194332_c.inventory;

	for (int i = 0; i < field_194336_g.getSizeInventory(); ++i) {
		ItemStack itemstack = field_194336_g.getStackInSlot(i);
		if (!itemstack.isEmpty()) {
			while (itemstack.getCount() > 0) {
				int j = inventoryplayer.storeItemStack(itemstack);
				if (j == -1) {
					j = inventoryplayer.getFirstEmptyStack();
				}

				ItemStack itemstack1 = itemstack.copy();
				itemstack1.setCount(1);
				inventoryplayer.add(j, itemstack1);
				field_194336_g.decrStackSize(i, 1);
			}
		}
	}

	field_194336_g.clear();
	field_194335_f.clear();
}

void ServerRecipeBookHelper::func_194329_b() {
	bool flag = field_194333_d.matches(field_194336_g, field_194332_c.world);
	int i = field_194331_b.getBiggestCraftableStack(field_194333_d, (IntList)null);
	if (flag) {
		bool flag1 = true;

		for (int j = 0; j < field_194336_g.getSizeInventory(); ++j) {
			ItemStack itemstack = field_194336_g.getStackInSlot(j);
			if (!itemstack.isEmpty() && Math.min(i, itemstack.getMaxStackSize()) > itemstack.getCount()) {
				flag1 = false;
			}
		}

		if (flag1) {
			return;
		}
	}

	int i1 = func_194324_a(i, flag);
	std::vector<int> intlist;
	if (field_194331_b.canCraft(field_194333_d, intlist, i1)) {
		int j1 = i1;

		for (auto k : intlist) {
			int l = RecipeItemHelper.unpack(k).getMaxStackSize();
			if (l < j1) {
				j1 = l;
			}
		}

		if (field_194331_b.canCraft(field_194333_d, intlist, j1)) {
			func_194326_a();
			func_194323_a(j1, intlist);
		}
	}

}

int ServerRecipeBookHelper::func_194324_a(int p_194324_1_, bool p_194324_2_) {
	int i = 1;
	if (field_194334_e) {
		i = p_194324_1_;
	}
	else if (p_194324_2_) {
		i = 64;

		for (int j = 0; j < field_194336_g.getSizeInventory(); ++j) {
			ItemStack itemstack = field_194336_g.getStackInSlot(j);
			if (!itemstack.isEmpty() && i > itemstack.getCount()) {
				i = itemstack.getCount();
			}
		}

		if (i < 64) {
			++i;
		}
	}

	return i;
}

void ServerRecipeBookHelper::func_194323_a(int p_194323_1_, IntList p_194323_2_) {
	int i = field_194336_g.getWidth();
	int j = field_194336_g.getHeight();
	if (field_194333_d instanceof ShapedRecipes) {
		ShapedRecipes shapedrecipes = (ShapedRecipes)field_194333_d;
		i = shapedrecipes.getWidth();
		j = shapedrecipes.getHeight();
	}

	int j1 = 1;
	Iterator iterator = p_194323_2_.iterator();

	for (int k = 0; k < field_194336_g.getWidth() && j != k; ++k) {
		for (int l = 0; l < field_194336_g.getHeight(); ++l) {
			if (i == l || !iterator.hasNext()) {
				j1 += field_194336_g.getWidth() - l;
				break;
			}

			Slot slot = (Slot)field_194337_h.get(j1);
			ItemStack itemstack = RecipeItemHelper.unpack((Integer)iterator.next());
			if (itemstack.isEmpty()) {
				++j1;
			}
			else {
				for (int i1 = 0; i1 < p_194323_1_; ++i1) {
					func_194325_a(slot, itemstack);
				}

				++j1;
			}
		}

		if (!iterator.hasNext()) {
			break;
		}
	}

}

void ServerRecipeBookHelper::func_194325_a(Slot p_194325_1_, ItemStack p_194325_2_) {
	InventoryPlayer inventoryplayer = field_194332_c.inventory;
	int i = inventoryplayer.findSlotMatchingUnusedItem(p_194325_2_);
	if (i != -1) {
		ItemStack itemstack = inventoryplayer.getStackInSlot(i).copy();
		if (!itemstack.isEmpty()) {
			if (itemstack.getCount() > 1) {
				inventoryplayer.decrStackSize(i, 1);
			}
			else {
				inventoryplayer.removeStackFromSlot(i);
			}

			itemstack.setCount(1);
			if (p_194325_1_.getStack().isEmpty()) {
				p_194325_1_.putStack(itemstack);
			}
			else {
				p_194325_1_.getStack().grow(1);
			}
		}
	}

}

bool ServerRecipeBookHelper::func_194328_c() {
	InventoryPlayer inventoryplayer = field_194332_c.inventory;

	for (int i = 0; i < field_194336_g.getSizeInventory(); ++i) {
		ItemStack itemstack = field_194336_g.getStackInSlot(i);
		if (!itemstack.isEmpty()) {
			int j = inventoryplayer.storeItemStack(itemstack);
			if (j == -1) {
				j = inventoryplayer.getFirstEmptyStack();
			}

			if (j == -1) {
				return false;
			}
		}
	}

	return true;
}