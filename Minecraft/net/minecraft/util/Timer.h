struct Timer
{
    int elapsedTicks;
    float renderPartialTicks;
    float elapsedPartialTicks;
    long lastSyncSysClock;
    float tickLength;

    Timer(float tps);
    void updateTimer();
};
