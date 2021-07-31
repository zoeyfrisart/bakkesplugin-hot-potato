/**
 * HotPotato, a Bakkesmod plugin to help practice bounce dribbling.
 * Copyright (c) 2021 Yannick Frisart
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "pch.h"
#include "HotPotato.h"

BAKKESMOD_PLUGIN(HotPotato, "Keep track of your bounce dribble practice", plugin_version, PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

// Score variables
int bounceCount = 0;
int streak = 0;
int streakRecord = 0;

void HotPotato::onLoad()
{
	_globalCvarManager = cvarManager;
	cvarManager->registerCvar("max_bounce", "2", "The max amount the ball is allowed to bounce", true, true, 1, true, 10, true);

	// Player touched the ball
	gameWrapper->HookEvent("Function TAGame.Ball_TA.OnCarTouch", [this](std::string eventName) {
		CVarWrapper maxBounceCvar = cvarManager->getCvar("max_bounce");
		if (!maxBounceCvar) { return; }
		int maxBounce = maxBounceCvar.getIntValue();

		if (bounceCount <= maxBounce) {
			// Player let the ball bounce
			// So we increase the streak by 1
			if (bounceCount > 0) {
				streak += 1;
			}
			
			// Set the bounceCount back to 0
			bounceCount = 0;
		}
		// The ball bounced more then allowed
		// So we reset the streak
		else {
			// If this is a new high score we update the high score
			if (streak > streakRecord) {
				streakRecord = streak;
			}

			// Reset the streak and set bounceCount back to 0
			streak = 0;
			bounceCount = 0;
		}
	});

	// Ball hit the world
	gameWrapper->HookEvent("Function TAGame.Ball_TA.OnHitWorld", [this](std::string eventName) {
		// Check if we are in free play
		if (!gameWrapper->IsInFreeplay()) { return; }

		// Get the server
		ServerWrapper server = gameWrapper->GetCurrentGameState();
		if (!server) { return; }

		// Get the ball
		BallWrapper ball = server.GetBall();
		if (!ball) { return; }

		// Get the location of the ball
		Vector ballLocation = ball.GetLocation();

		CVarWrapper maxBounceCvar = cvarManager->getCvar("max_bounce");
		if (!maxBounceCvar) { return; }
		int maxBounce = maxBounceCvar.getIntValue();

		// The interaction with the world is either on the floor or low side of the wall
		if (ballLocation.Z < 200) {
			// Increase the bounce count by 1
			bounceCount += 1;

			// Check if the bounce count is higher then the max bounce count
			if (bounceCount > maxBounce) {
				if (streak > streakRecord) {
					streakRecord = streak;
				}

				streak = 0;
			}
		}
	});
}

void HotPotato::onUnload()
{
}

int HotPotato::getBallStreak() {
	return streak;
}

int HotPotato::getBounceCount() {
	return bounceCount;
}

int HotPotato::getBallStreakRecord() {
	return streakRecord;
}

void HotPotato::resetHighscore() {
	streakRecord = 0;
}