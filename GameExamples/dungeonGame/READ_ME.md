## Short overview
This application is an example of a game client and server using the Gaijin Application Platfrom (initials GAP). For simplicity, communication between the client and the server is implemented through direct calls that simulate client-server API interaction. The game client has a simple text interface.

## Running and authorisation
[See build requirements below](#build-requirements)

The application is launched from the command shell without any parameters. Browser opens automatically with login page. It is possible to use different types of accounts:  
https://login.gaijin.net/ru/sso/login  
After successfully logging in, switch back to the shell.
```sh
~$dungeon_game
You are entering the terrible dungeon...
```

## Gameplay
The gameplay is simplified as much as possible to focus on working with GAP API.

There is a typical dungeon. Player meets enemies and has two options: escape or fight. If you decide to fight you always win killing the enemy by one hit. The escaping ends the game.

You can kill opponents indefinitely, the only challenge here is your patience. =) After completing the game, you receive rewards based on your gained exp, players leaderboard is displayed.

## Gaijin Application Platfrom API
### Terms: stats and unlocks
"Stat" is an element of user's statictic data. Values of kills count, experiens points, levels - it is all about stat. The "unlock" can be considered as an element of the reward system. It consists of stages, which are unlocked when some condition is done. Stage can give some rewards, e.g. increase a prized stat. Stats and unlocks are configured via GAP web interface. Also you can download/upload them as json files (an example see below).

These example game have several stats and unlocks.
The stats are:
- *kills* - number of defeated enemys
- *level* - character level
- *exp* - gained experience
- *gems* - some in-game currency

The unlocks are:
- **levels**
- **gems**

### How it works?
During the game session player kills the enemys. Defeating enemy gains some exp (dependent of enemy characteristic). This part is a buisness logic of the game. At the session's end server sends the numbers of kills and exp with *ChangeStats* method of GAP API.

You can seen in the next section the configs, which describes the next following mechanism.

Incrementing of the *exp* stat affects on the **level** unlock. First 3 points unlock the first stage, which increase *level* stat by one point. Next 7 points make the summary exp 10 and the second stage of the **level** unlock increases *level* stat by another point. This means character now have level 2. And finally every next 25 exp points will give level up, because **level** unlock has option *startStageLoop* with value 3, what means repeat the third stage.

So, if the character has *level* 0 and receives 35 points at once, he reaches *level* 3. This happens automatically, because the **level** unlock has the option *autoRewarding*.

Changes of the *level* stat, in turn, affects the **gems** unlock. But unlike the **levels**, this award configured for receiving manually. It comes with following steps:
- *GetUnlocks* method gets a descrition of the ulocks current states
- Checking *lastRewardedStage* and *stage* for **gems**: if *lastRewardedStage* < *stage*, player gets message "You have awards, receive now? (**y/n**)"
- If **y** is chosen, the method *GrantRewards* sequentially unlocks all stages

As seen from config below, every two levels give one gem.

### Configs
<details>
<summary><b>Stats config</b></summary>

```json
[
  {
    "name": "level",
    "type": "INT",
    "onlyIncrement": true,
    "leaderboard": true
  },
  {
    "name": "exp",
    "type": "INT",
    "onlyIncrement": true,
    "leaderboard": true
  },
  {
    "name": "kills",
    "type": "INT",
    "onlyIncrement": true,
    "leaderboard": true
  },
  {
    "name": "gems",
    "type": "INT",
    "onlyIncrement": true
  }
]
```
</details>

<details>
<summary><b>Unlocks config</b></summary>

```json
[
  {
    "name": "gems",
    "table": "global",
    "mode": "default",
    "periodic": true,
    "startStageLoop": 1,
    "condition": "s.level",
    "stages": [
      {
        "progress": 2,
        "updStats": [
          {
            "mode": "default",
            "name": "gems",
            "value": 1,
            "type": "ADD"
          }
        ]
      }
    ],
    "type": "NORMAL"
  },
  {
    "name": "level",
    "table": "global",
    "mode": "default",
    "periodic": true,
    "startStageLoop": 3,
    "condition": "s.exp",
    "autoRewarding": true,
    "stages": [
      {
        "progress": 3,
        "updStats": [
          {
            "mode": "default",
            "name": "level",
            "value": 1,
            "type": "ADD"
          }
        ]
      },
      {
        "progress": 10,
        "updStats": [
          {
            "mode": "default",
            "name": "level",
            "value": 1,
            "type": "ADD"
          }
        ]
      },
      {
        "progress": 25,
        "updStats": [
          {
            "mode": "default",
            "name": "level",
            "value": 1,
            "type": "ADD"
          }
        ]
      }
    ],
    "type": "NORMAL"
  }
]
```
</details>


## Code overview
The project has several base parts: libs, gapApi, game client, game server. **Just copy the */gapApi*, */libs* and *config.h* to use GAP API as a start template in your application.**

### libs
This is just third party libs.

### gapApi
As seen from the title, this is a implemetation of some GAP API methods:
- *GetStats, GetUnlocks, GrantRewards, ChangeStats* - for interaction with user's stats and unlocks;
- *cln_get_leaderboard* - for access to the leaderboard

### server
This part imitates a backend application. It realizes some game engine: enemies generation, battle mechanics, etc. Also it sends game session statistics to the GAP.

### gameClient
This is a user-side application. It provides several features for user:
- game process interface
- reward interface (using GAP API)
- leaderboard showing (using GAP API)

## Build requirements
- Cmake, version >= 3.4.0 (prior ones may also be suitable)  
https://cmake.org/download/
- OpenSSL, version >= 1.1.1  
https://www.openssl.org/source/  
https://wiki.openssl.org/index.php/Binaries

## Links
More details on the Gaijin application platform wiki:
https://github.com/GaijinEntertainment/GaijinApplicationPlatform/wiki
