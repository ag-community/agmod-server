
// New resets won't be counted until at least this time passed since the last reset
constexpr float ATTEMPT_MIN_TIME = 0.5f;

namespace SpeedrunStats
{
void Init();
void AddJump();
void AddDuck();
void AddDistance(float* dist);
void AddLoad();
void PrintSession();
void PrintRun();
void ShowRunTime();
void EndRun();
}
