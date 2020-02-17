#include <memory>
class NBTTagList;

class Rotations
{
public:
	Rotations(float x, float y, float z);
	explicit Rotations(NBTTagList* nbt);
	std::shared_ptr<NBTTagList> writeToNBT() const;

	friend bool operator==(const Rotations& a, const Rotations& b);

	float getX() const;
	float getY() const;
	float getZ() const;

protected:
	float x;
	float y;
	float z;
};


