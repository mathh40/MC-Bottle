class IGrowable {
public:
    virtual bool canGrow(World &var1, BlockPos &var2, IBlockState &var3, bool var4) = 0;
    virtual bool canUseBonemeal(World &var1, pcg32 &var2, BlockPos &var3, IBlockState &var4) = 0;
    virtual void grow(World &var1, pcg32 &var2, BlockPos &var3, IBlockState &var4) = 0;
protected:
    ~IGrowable() = default;

};
