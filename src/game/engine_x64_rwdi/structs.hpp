#pragma once

namespace game
{
    namespace engine_x64_rwdi
    {
        namespace CampaignManager
        {
            namespace impl
            {
                struct CampaignManager
                {
                    void*   vftable;            // +0x00
                    void*   unknown0;           // +0x08
                    void*   vftable2;           // +0x10
                    void*   unknown1;           // +0x18
                    int     vector1_capacity;   // +0x20  = 30
                    int     pad0;               // +0x24
                    int     currentCampaignId;  // +0x28  = INVALID initially
                    int     pendingCampaignId;  // +0x2C  = INVALID initially
                    uint8_t flag;               // +0x30  = 1
                };
            }
        }

        namespace PlayerDI
        {
            namespace impl
            {
                struct PlayerDI_vftable
                {

                };

                #pragma pack(push, 1)
                struct PlayerDI
                {
                    PlayerDI_vftable* vtable;
                    char _pad1[3508];
                    uint32_t m_ControllerGroupSpecialization;
                    char _pad2[160789];
                };
                #pragma pack(pop)

                static_assert(sizeof(PlayerDI) == 164309, "PlayerDI size mismatch! Expected 164309 bytes.");
            }   
        }

        namespace ILevel
        {
            namespace impl
            {
                struct ILevel
                {
                    void* vtable;    
                    uint8_t pad[3180];
                    char levelName[64];
                };
            }

            typedef const char*(__fastcall* t_GetLevelName)(impl::ILevel* level);
            inline t_GetLevelName org_GetLevelName = nullptr;
        }

        namespace Sessions
        {
            namespace StatusDL
            {
                enum class ETeam : int
                {
                    None=0,
                    Default=1,
                    Zombie=2
                };

                enum class EPermission : int 
                { 
                    Refused=-1, 
                    Undefined=0, 
                    Granted=1 
                };

                enum class EStatus : int 
                {
                    NotConnected=0, 
                    Connecting=1,
                    Connected_LoadingLevel=2, 
                    Connected_LevelLoaded=3, 
                    Disconnecting=4,
                    Invalid = 5
                };

                enum class ELobbyNAT : int 
                { 
                    NAT_1=0, 
                    NAT_2=1, 
                    NAT_3=2, 
                    NAT_Unknown=3 
                };

                enum class EDisconnectReason : int
                {
                    None = 0,
                    ReplicationNoMaster = 1,
                    ReplicationInvalidVersion = 2,
                    ReplicationInvalidData = 3,
                    ReplicationNoInitialized = 4,

                    LobbySessionJoinFailure_tmp = 5,
                    LobbySessionCreateFailure_tmp = 6,
                    LobbyGameNotJoinableFull = 7,
                    LobbyDropOut = 8,
                    LobbyDropOutForcedByFinale = 9,
                    LobbyQuit = 10,
                    LobbyDropOutStayInParty = 11,
                    LobbyMigrationOnMasterFailed = 12,
                    LobbyMigrationOnClientFailed = 13,
                    LobbyJoinRemoteClientFailure = 14,
                    LobbyModifySessionFailed = 15,

                    NetNetworkError = 16,
                    NetLostMasterConnectingFailed = 17,
                    NetNoMasterResponse = 18,
                    NetLostMasterNoMigration = 19,
                    NetUnexpectedProblem = 20,
                    NetAssignSlotFailed_GameFull = 21,
                    NetAssignSlotFailed_NoFreePublicSlot = 22,
                    NetLeaving = 23,
                    NetMsgConnectivityFailure = 24,
                    NetMsgRequesterOtherPeerFailed = 25,
                    NetMsgRequesterP2PFailed = 26,
                    NetMsgRequesterNotAllowed = 27,
                    NetMsgPeerAlreadyLeft = 28,
                    NetMsgConnectionUnsuccessfull = 29,

                    GameDestroying = 30,
                    GameNotJoinable = 31,
                    SessionRoomDestroyed = 32,
                    ProfileOffline = 33,
                    AppSuspension = 34,

                    UserDisconnectReason01 = 35,
                    UserDisconnectReason02 = 36,
                    UserDisconnectReason03 = 37,
                    UserDisconnectReason04 = 38,
                    UserDisconnectReason05 = 39,
                    UserDisconnectReason06 = 40,
                    UserDisconnectReason07 = 41,
                    UserDisconnectReason08 = 42,
                    UserDisconnectReason09 = 43,
                    UserDisconnectReason10 = 44,
                    UserDisconnectReason11 = 45,
                    UserDisconnectReason12 = 46,
                    UserDisconnectReason13 = 47,
                    UserDisconnectReason14 = 48,
                    UserDisconnectReason15 = 49,
                    UserDisconnectReason16 = 50,
                    UserDisconnectReason17 = 51,
                    UserDisconnectReason18 = 52,
                    UserDisconnectReason19 = 53,
                    UserDisconnectReason20 = 54,
                    UserDisconnectReason21 = 55,
                    UserDisconnectReason22 = 56,
                    UserDisconnectReason23 = 57,
                    UserDisconnectReason24 = 58,
                    UserDisconnectReason25 = 59,

                    GameSessionIsStarting = 60,
                    GameSessionIsFinishing = 61,
                    GameSessionNoSpaceInTeam = 62,
                    GameSessionStatusTimeout = 63,

                    SteamAuthVACBanned = 64,
                    SteamAuthNoAuthorization = 65,
                    SteamAuthTicketCanceled = 66,
                    SteamAuthTimedOut = 67,
                    SteamAuthNoLicence = 68,
                    SteamAuthGameNotSecured = 69,
                    SteamAuthGameNotLicenced = 70,
                    SteamAuthGameBanned = 71,
                    SteamAuthGameNotAuthorized = 72,

                    PartyUnexpectedError = 73,
                    PartyUnknownError = 74,
                    PartyActiveUserChanged = 75,
                    PartyNoLobbySession = 76,
                    PartyNoPartyCreated = 77,
                    PartyCreateGameSessionFailure = 78,
                    PartyJoinGameSessionFailure = 79,
                    PartyJoinedInvalidGameSession = 80,
                    PartyGameSessionLeft = 81,
                    PartyLeft = 82,
                    PartyNetworkDisconnected = 83,
                    PartyGameNotJoinableFull = 84,
                    PartyGameNotJoinableNetworkProblem = 85,
                    PartyGameNotJoinableInvalidSession = 86,

                    LobbyDlcAreaNotAvailable = 87,
                    LobbyMapNotInstalled = 88,
                    AgeRestriction = 89,
                    GamePatchRequired = 90,

                    UserDisconnectReason26 = 91,
                    UserDisconnectReason27 = 92,

                    PartyCantInviteError = 93,
                    PartyNoSessionObtainedError = 94,
                    PartyNoMasterSessionReferenceError = 95,
                    PartyNoMasterInfoError = 96,
                    PartyJoinPartySessionError = 97,

                    NetNoMasterResponseNotRecognizedData = 98,
                    UserDisconnectReason28 = 99,

                    Unknown = 100,
                    LobbySignalingActivationFail = 101,
                    LobbyJoiningRoomTimeout = 102,
                    UserDisconnectReason29 = 103,

                    Undefined = 104
                };

                std::string ETeam_to_string(ETeam Type);

                namespace impl
                {
                    struct StatusDL 
                    {
                        void* GameObject;
                        uint8_t gameobject[0x38];

                        uint8_t _pad3[0x4];

                        EStatus        m_Status;               // 0x44
                        uint8_t        _pad0[0x8];             // 0x48-0x4F
                        ELobbyNAT      m_NatType;              // 0x50
                        uint8_t        _pad1[0x4];             // 0x54-0x57

                        // Sessions::StatusDL fields
                        unsigned short m_HumanTeamSlots[4];    // 0x58
                        unsigned short m_ZombieTeamSlot;       // 0x60
                        uint8_t        _pad2[0x2];             // 0x62
                        ETeam          m_Team;                 // 0x64
                        EPermission    m_ZombieJoinPermission; // 0x68
                        EPermission    m_JoinPermission;       // 0x6C
                        unsigned int   m_TrackedQuestId;       // 0x70
                        short          m_ZombieAllowedResult;  // 0x74
                    };
                }
            }
        }

        namespace IGame
        {
            namespace impl
            {
                struct IGame_vftable;

                struct IGame
                {
                    IGame_vftable* vtable; // 0000000181823520 gamedll
                    void* _this;
                };

                struct IGame_vftable 
                {
                    void(__fastcall* Unk0)();
                    void(__fastcall* Unk1)();
                    void(__fastcall* Unk2)();
                    void(__fastcall* Unk3)();
                    void(__fastcall* Unk4)();
                    void(__fastcall* Unk5)();
                    void(__fastcall* Unk6)();
                    void(__fastcall* Unk7)();
                    void(__fastcall* Unk8)();
                    void(__fastcall* Unk9)();
                    void(__fastcall* Unk10)();
                    void(__fastcall* Unk11)();
                    void(__fastcall* Unk12)();
                    void(__fastcall* Unk13)();
                    void(__fastcall* Unk14)();
                    void(__fastcall* Unk15)();
                    void(__fastcall* Unk16)();
                    void(__fastcall* Unk17)();
                    void(__fastcall* Unk18)();
                    void(__fastcall* Unk19)();
                    void(__fastcall* Unk20)();
                    void(__fastcall* Unk21)();
                    void(__fastcall* Unk22)();
                    void(__fastcall* Unk23)();
                    void(__fastcall* Unk24)();
                    void(__fastcall* Unk25)();
                    void(__fastcall* Unk26)();
                    void(__fastcall* Unk27)();
                    void(__fastcall* Unk28)();
                    void(__fastcall* Unk29)();
                    void(__fastcall* Unk30)();
                    void(__fastcall* Unk31)();
                    void(__fastcall* Unk32)();
                    void(__fastcall* Unk33)();
                    void(__fastcall* Unk34)();
                    void(__fastcall* Unk35)();
                    void(__fastcall* Unk36)();
                    void(__fastcall* Unk37)();
                    void(__fastcall* Unk38)();
                    void(__fastcall* Unk39)();
                    void(__fastcall* Unk40)();
                    void(__fastcall* Unk41)();
                    void(__fastcall* Unk42)();
                    void(__fastcall* Unk43)();
                    void(__fastcall* Unk44)();
                    void(__fastcall* Unk45)();
                    void(__fastcall* Unk46)();
                    void(__fastcall* Unk47)();
                    void(__fastcall* Unk48)();
                    void(__fastcall* Unk49)();
                    void(__fastcall* Unk50)();
                    void(__fastcall* Unk51)();
                    void(__fastcall* Unk52)();
                    void(__fastcall* Unk53)();
                    void(__fastcall* Unk54)();
                    void(__fastcall* Unk55)();
                    void(__fastcall* Unk56)();
                    void(__fastcall* Unk57)();
                    void(__fastcall* Unk58)();
                    void(__fastcall* Unk59)();
                    void(__fastcall* Unk60)();
                    void(__fastcall* Unk61)();
                    void(__fastcall* Unk62)();
                    void(__fastcall* Unk63)();
                    void(__fastcall* Unk64)();
                    void(__fastcall* Unk65)();
                    void(__fastcall* Unk66)();
                    void(__fastcall* Unk67)();
                    void(__fastcall* Unk68)();
                    void(__fastcall* Unk69)();
                    void(__fastcall* Unk70)();
                    void(__fastcall* Unk71)();
                    void(__fastcall* Unk72)();
                    void(__fastcall* Unk73)();
                    void(__fastcall* Unk74)();
                    void(__fastcall* Unk75)();
                    void(__fastcall* Unk76)();
                    void(__fastcall* Unk77)();
                    void(__fastcall* OnLobbyRemotePeerJoined)(IGame* this_, unsigned int peer_id);  
                    void(__fastcall* OnLobbyRemotePeerLeft)(IGame* this_, unsigned int peer_id, Sessions::StatusDL::EDisconnectReason disconnect_reason);  
                    void(__fastcall* OnLobbyRemotePeerStartedJoining)(IGame* this_, unsigned int peer_id, Sessions::StatusDL::ETeam team);
                    void(__fastcall* OnLobbyRemotePeerFailedJoining)(IGame* this_,  unsigned int peer_id, Sessions::StatusDL::EDisconnectReason disconnect_reason);
                    void(__fastcall* Unk82)();
                    void(__fastcall* Unk83)();
                    void(__fastcall* Unk84)();
                    void(__fastcall* Unk85)();
                    void(__fastcall* Unk86)();
                    void(__fastcall* Unk87)();
                    void(__fastcall* Unk88)();
                    void(__fastcall* Unk89)();
                    void(__fastcall* Unk90)();
                    void(__fastcall* Unk91)();
                    void(__fastcall* Unk92)();
                    void(__fastcall* Unk93)();
                    void(__fastcall* Unk94)();
                    void(__fastcall* Unk95)();
                    void(__fastcall* Unk96)();
                    void(__fastcall* Unk97)();
                    void(__fastcall* Unk98)();
                    void(__fastcall* Unk99)();
                    void(__fastcall* Unk100)();
                    void(__fastcall* Unk101)();
                    void(__fastcall* Unk102)();
                    void(__fastcall* Unk103)();
                    void(__fastcall* Unk104)();
                    void(__fastcall* Unk105)();
                    void(__fastcall* Unk106)();
                    void(__fastcall* Unk107)();
                    void(__fastcall* Unk108)();
                    void(__fastcall* Unk109)();
                    void(__fastcall* Unk110)();
                    void(__fastcall* Unk111)();
                    void(__fastcall* Unk112)();
                    void(__fastcall* Unk113)();
                    void(__fastcall* Unk114)();
                    void(__fastcall* Unk115)();
                    void(__fastcall* Unk116)();
                    void(__fastcall* Unk117)();
                    void(__fastcall* Unk118)();
                    void(__fastcall* Unk119)();
                    void(__fastcall* Unk120)();
                    void(__fastcall* Unk121)();
                    void(__fastcall* Unk122)();
                    void(__fastcall* Unk123)();
                    void(__fastcall* Unk124)();
                    void(__fastcall* Unk125)();
                    void(__fastcall* Unk126)();
                    void(__fastcall* Unk127)();
                    void(__fastcall* Unk128)();
                    void(__fastcall* Unk129)();
                    void(__fastcall* Unk130)();
                    void(__fastcall* Unk131)();
                    void(__fastcall* Unk132)();
                    void(__fastcall* Unk133)();
                    void(__fastcall* Unk134)();
                    void(__fastcall* Unk135)();
                    void(__fastcall* Unk136)();
                    void(__fastcall* Unk137)();
                    void(__fastcall* Unk138)();
                    void(__fastcall* Unk139)();
                    void(__fastcall* Unk140)();
                    void(__fastcall* Unk141)();
                    void(__fastcall* Unk142)();
                    void(__fastcall* Unk143)();
                    void(__fastcall* Unk144)();
                    void(__fastcall* Unk145)();
                    void(__fastcall* Unk146)();
                    void(__fastcall* Unk147)();
                    void(__fastcall* Unk148)();
                    void(__fastcall* Unk149)();
                    void(__fastcall* Unk150)();
                    void(__fastcall* Unk151)();
                    void(__fastcall* Unk152)();
                    void(__fastcall* Unk153)();
                    void(__fastcall* Unk154)();
                    void(__fastcall* Unk155)();
                    void(__fastcall* Unk156)();
                    void(__fastcall* Unk157)();
                    void(__fastcall* OnGameSuspending)();  
                    void(__fastcall* OnGameResuming)();   
                    void(__fastcall* Unk160)();
                    void(__fastcall* Unk161)();
                    void(__fastcall* Unk162)();
                    void(__fastcall* Unk163)();
                    void(__fastcall* Unk164)();
                    void(__fastcall* Unk165)();
                    void(__fastcall* Unk166)();
                    void(__fastcall* Unk167)();
                    void(__fastcall* Unk168)();
                    void(__fastcall* Unk169)();
                    void(__fastcall* Unk170)();
                    void(__fastcall* Unk171)();
                    void(__fastcall* Unk172)();
                    void(__fastcall* Unk173)();
                    void(__fastcall* Unk174)();
                    void(__fastcall* Unk175)();
                    void(__fastcall* Unk176)();
                    void(__fastcall* Unk177)();
                    void(__fastcall* Unk178)();
                    void(__fastcall* Unk179)();
                    void(__fastcall* Unk180)();
                    void(__fastcall* Unk181)();
                    void(__fastcall* Unk182)();
                    void(__fastcall* Unk183)();
                    void(__fastcall* Unk184)();
                    void(__fastcall* Unk185)();
                    void(__fastcall* Unk186)();
                    void(__fastcall* Unk187)();
                    void(__fastcall* Unk188)();
                    void(__fastcall* Unk189)();
                    void(__fastcall* Unk190)();
                    void(__fastcall* Unk191)();
                    void(__fastcall* Unk192)();
                    void(__fastcall* Unk193)();
                    void(__fastcall* Unk194)();
                    void(__fastcall* Unk195)();
                    void(__fastcall* Unk196)();
                    void(__fastcall* Unk197)();
                    void(__fastcall* Unk198)();
                    void(__fastcall* Unk199)();
                    void(__fastcall* Unk200)();
                    void(__fastcall* Unk201)();
                    void(__fastcall* Unk202)();
                    void(__fastcall* Unk203)();
                    void(__fastcall* Unk204)();
                    void(__fastcall* Unk205)();
                    void(__fastcall* Unk206)();
                    void(__fastcall* Unk207)();
                    void(__fastcall* Unk208)();
                    void(__fastcall* Unk209)();
                    void(__fastcall* Unk210)();
                    void(__fastcall* Unk211)();
                    void(__fastcall* Unk212)();
                    void(__fastcall* Unk213)();
                    void(__fastcall* Unk214)();
                    void(__fastcall* Unk215)();
                    void(__fastcall* Unk216)();
                    void(__fastcall* Unk217)();
                    void(__fastcall* Unk218)();
                    void(__fastcall* Unk219)();
                    void(__fastcall* Unk220)();
                    void(__fastcall* Unk221)();
                    void(__fastcall* Unk222)();
                    void(__fastcall* Unk223)();
                    void(__fastcall* Unk224)();
                };
            }
        }
    }
}