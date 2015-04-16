Msg("Initiating Crows Script\n");

DirectorOptions <-
{
MobSpawnMinTime = 3
MobSpawnMaxTime = 7
MobMinSize = 20
MobMaxSize = 30
MobMaxPending = 30
SustainPeakMinTime = 5
SustainPeakMaxTime = 10
IntensityRelaxThreshold = 0.99
RelaxMinInterval = 1
RelaxMaxInterval = 5
RelaxMaxFlowTravel = 50
SpecialRespawnInterval = 1.0
PreferredMobDirection = SPAWN_ANYWHERE
ZombieSpawnRange = 2000
}

Director.ResetMobTimer()
Director.PlayMegaMobWarningSounds()