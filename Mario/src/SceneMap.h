#pragma once

static const uint32_t s_MapWidth = 212 * 2;
/*
 G : Ground
 | : Castel Brick
 o : Castel Gate
 u : castel Gate Domb
 . : Castel Domb
 l : Castel Windlow Left
 r : Castel Window Right
 S : Steps
 - : Bridge
 ! : Pipe Base
 Y : Pipe Head
 X : Bricks
 B : Bonus
 */
static const char* s_MapTiles =
"                                                                                                                                                                                                                                                                                                                                                                                                                                        "
"                                                                                                                                                                                                                                                                                                                                                                                                                                  .     "
"                 B                  XXXX                             B            XBX                                                            B                                    B                               XXXXXXXX     XXXXX           B           XXXXX XX  XX XXXXX                                                                           B                                                                    ...    "
"                                                                                                                                                                                                                                                                                                                                                                                                                                 l|r    "
"                                                                                                                                                                                                                                                                                                                                                                                                     S                         .......  "
"                                                                                                                                                                                                                                                                                                                                                                                                    SS                         |||||||  "
"                                                                                                                                                                                     SS                                                                                                                                                                                                            SSS                         |l|u|r|  "
"               XBXBX              XBXXXXBX                           S           XXXXX               B                                           S                                  SSS                            XXXX      XXXX             B    B    B             XXXX                                                                                XBXBX                                   SSSS                         |||o|||  "
"                                                  Y                 SS                                                                          SS                                 SSSS                                                                                                                                           S  S                                                           SSSSS                       ..........."
"                                                  !                SSS                                                   S  S                  SSS                                SSSSS                                                                                                                      Y                   SS  SS                                                         SSSSSS                       |||||||||||"
"                            Y                     !               SSSS                               Y                  SS  SS                SSSS             Y                 SSSSSS                                                                                                                      !                  SSS  SSS                    Y                                  SSSSSSS                       ||u||u||u||"
"                            !       S  S          !              SSSSS                               !                 SSS  SSS              SSSSS             !                SSSSSSS                                                                                                                      !                 SSSS  SSSS                   !                                 SSSSSSSS             S         ||o||o||0||"
"GGGGGG GG GGGGGGGGGGGGGGGGGGGGGGGGGGG  GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG--------------------GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG  GGGGGGGGGGGGGGGGGGGGGG  GGGGGGGGGGGGGGGGGGG GGGGGGGGGGGGGGG  GGGGGGGGGGGGG-----------------------------------------------------------------------------------------------GGGGGGGGGGGGGGGGGGGGGGGGGGGGGG  GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG  GGGGGGGGGGGGGGGGG  GGGGGGGGGGGGG"
"GGGGGG GG GGGGGGGGGGGGGGGGGGGGGGGGGGG  GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG                    GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG  GGGGGGGGGGGGGGGGGGGGGG  GGGGGGGGGGGGGGGGGGG GGGGGGGGGGGGGGG  GGGGGGGGGGGGG                                                                                               GGGGGGGGGGGGGGGGGGGGGGGGGGGGGG  GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG  GGGGGGGGGGGGGGGGG  GGGGGGGGGGGGG"
;

