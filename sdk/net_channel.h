/*
 * net_channel.h
 */
#pragma once

// todo : clean up
// from : fuzion

// class INetMessage;
// class INetChannelHandler;

#define FLOW_OUTGOING    0
#define FLOW_INCOMING    1
#define MAX_FLOWS        2        // in & out

class net_channel_info_t
{
public:
    enum
    {
        GENERIC = 0,    // must be first and is default group
        LOCALPLAYER,    // bytes for local player entity update
        OTHERPLAYERS,    // bytes for other players update
        ENTITIES,        // all other entity bytes
        SOUNDS,            // game sounds
        EVENTS,            // event messages
        USERMESSAGES,    // user messages
        ENTMESSAGES,    // entity messages
        VOICE,            // voice data
        STRINGTABLE,    // a stringtable update
        MOVE,            // client move cmds
        STRINGCMD,        // string command
        SIGNON,            // various signondata
        TOTAL,            // must be last and is not a real group
    };

    virtual const char* get_address() const = 0; // get channel IP address as string
    virtual float   get_time() const = 0;    // current net time
    virtual float   get_time_connected() const = 0;    // get connection time in seconds
    virtual int     get_buffer_size() const = 0;    // netchannel packet history size
    virtual int 	get_data_rate() const = 0; // send data rate in byte/sec

    virtual bool 	is_loopback() const = 0;    // true if loopback channel
    virtual bool    is_timing_out() const = 0;    // true if timing out
    virtual bool 	is_playback() const = 0;    // true if demo playback

    virtual float   get_latency(int flow) const = 0;     // current latency (RTT), more accurate but jittering
    virtual float 	get_avg_latency(int flow) const = 0; // average packet latency in seconds
    virtual float   get_avg_loss(int flow) const = 0;     // avg packet loss[0..1]
    virtual float 	get_avg_choke(int flow) const = 0;     // avg packet choke[0..1]
    virtual float   get_avg_data(int flow) const = 0;     // data flow in bytes/sec
    virtual float   get_avg_packets(int flow) const = 0; // avg packets/sec
    virtual int     get_total_data(int flow) const = 0;     // total flow in/out in bytes
    virtual int     get_sequence_nr(int flow) const = 0;    // last send seq number
    virtual bool 	is_valid_packet(int flow, int frame_number) const = 0; // true if packet was not lost/dropped/chocked/flushed
    virtual float 	get_packet_time(int flow, int frame_number) const = 0; // time when packet was send
    virtual int 	get_packet_bytes(int flow, int frame_number, int group) const = 0; // group size of this packet
    virtual bool 	get_stream_progress(int flow, int* received, int* total) const = 0;  // TCP progress if transmitting
    virtual float 	get_time_since_last_received() const = 0;    // get time since last recieved packet in seconds
    virtual float 	get_command_interpolation_amount(int flow, int frame_number) const = 0;
    virtual void    get_packet_response_latency(int flow, int frame_number, int* pnLatencyMsecs, int* pnChoke) const = 0;
    virtual void    get_remote_framerate(float* pflFrameTime, float* pflFrameTimeStdDeviation) const = 0;
    virtual float   get_timeout_seconds() const = 0;
    virtual const char* get_name() const = 0;    // get channel name
};

//typedef struct netpacket_s netpacket_t;
//typedef struct netadr_s    netadr_t;

enum net_msg_type_t
{
    MSG_NOP = 0,
    MSG_DISCONNECT = 1,
    MSG_FILE = 2,
    MSG_SPLITSCREENUSER = 3,
    MSG_TICK = 4,
    MSG_STRINGCMD = 5,
    MSG_SETCONVAR = 6,
    MSG_SIGNONSTATE = 7,
    MSG_CLIENTINFO = 8,         // client info (table CRC etc)
    MSG_MOVE = 9,               // [CUserCmd]
    MSG_VOICEDATA = 10,         // Voicestream data from a client
    MSG_BASELINEACK  = 11,      // client acknowledges a new baseline seqnr
    MSG_LISTENEVENTS = 12,      // client acknowledges a new baseline seqnr
    MSG_RESPONDCVARVALUE = 13,  // client is responding to a svc_GetCvarValue message.
    MSG_FILECRCCHECK = 14,      // client is sending a file's CRC to the server to be verified.
    MSG_LOADINGPROGRESS = 15,
    MSG_SPLITPLAYERCONNECT = 16,
    MSG_CLIENTMESSAGE = 17,
    MSG_CMDKEYVALUES = 18,
    MSG_HLTV_REPLAY = 20,
    MSG_HLTV_FIXUPOPERATORTICK = 21,
    MSG_HLTV_FIXUPOPERATORRECEIVER = 22,
};

//class net_channel_t : public net_channel_info_t
//{
//public:
//    virtual ~net_channel_t( void ) {}; // 27
//    virtual void Destroy( void ) = 0; // calls destructor.
//    virtual void SetDataRate( float rate ) = 0;
//    virtual void __unknown1( void ) = 0; // 30
//    virtual bool RegisterMessage( INetMessage* msg ) = 0;
//    virtual bool StartStreaming( unsigned int challengeNr ) = 0;
//    virtual void ResetStreaming( void ) = 0;
//    virtual void SetTimeout( float seconds ) = 0;
//    virtual void SetDemoRecorder(/*IDemoRecorder*/void *recorder) = 0; // 35
//    virtual void SetChallengeNr( unsigned int chnr ) = 0;
//    virtual void Reset( void ) = 0;
//    virtual void Clear( void ) = 0;
//    virtual void Shutdown( const char* reason ) = 0;
//    virtual void ProcessPlayback( void ) = 0; // 40
//    virtual bool ProcessStream( void ) = 0;
//    virtual void ProcessPacket( /*struct netpacket_s*/void* packet, bool bHasHeader ) = 0;
//    virtual bool SendNetMsg( INetMessage& msg, bool bForceReliable = false, bool bVoice = false ) = 0;
//    virtual bool SendData(/*bf_write&*/void *msg, bool bReliable = true) = 0;
//    virtual bool SendFile( const char* filename, unsigned int transferID ) = 0; // 45
//    virtual void DenyFile( const char* filename, unsigned int transferID ) = 0;
//    virtual void RequestFile_OLD( const char* filename, unsigned int transferID ) = 0; // DEFUNCT. Errors when called.
//    virtual void SetChoked( void ) = 0;
//    virtual int  SendDatagram(/*bf_write*/void *data) = 0;
//    virtual bool Transmit( bool onlyReliable = false ) = 0; // 50
//    virtual void /*const netadr_t& */GetRemoteAddress( void ) const = 0;
//    virtual INetChannelHandler* GetMsgHandler( void ) const = 0;
//    virtual int GetDropNumber( void ) const = 0;
//    virtual int GetSocket( void ) const = 0;
//    virtual unsigned int GetChallengeNr( void ) const = 0; // 55
//    virtual void GetSequenceData( int& nOutSequenceNr, int& nInSequenceNr, int& nOutSequenceNrAck ) = 0;
//    virtual void SetSequenceData( int nOutSequenceNr, int nInSequenceNr, int nOutSequenceNrAck ) = 0;
//    virtual void UpdateMessageStats( int msggroup, int bits ) = 0;
//    virtual bool CanPacket( void ) const = 0;
//    virtual bool IsOverflowed( void ) const = 0; // 60
//    virtual bool IsTimedOut( void ) const = 0;
//    virtual bool HasPendingReliableData( void ) = 0;
//    virtual void SetFileTransmissionMode( bool bBackgroundMode ) = 0;
//    virtual void SetCompressionMode( bool bUseCompression ) = 0; // 64
//    virtual unsigned int RequestFile( const char* filename ) = 0;
//    virtual void SetMaxBufferSize( bool bReliable, int nBytes, bool bVoice = false ) = 0; // 66
//    /* VIRTUAL SOUP START */
//    virtual void __unknown2( void ) = 0;
//    virtual void __unknown3( void ) = 0;
//    virtual void __unknown4( void ) = 0;
//    virtual void __unknown5( void ) = 0;
//    /* CONTAINS SOME OF THESE
//     virtual float GetTimeSinceLastReceived( void ) const = 0;    // get time since last received packet in seconds
//     virtual bool IsNull() const = 0;
//     virtual int GetNumBitsWritten( bool bReliable ) = 0;
//     virtual void SetInterpolationAmount( float flInterpolationAmount ) = 0;
//     virtual void SetRemoteFramerate( float flFrameTime, float flFrameTimeStdDeviation ) = 0;
//     */
//    // Max # of payload bytes before we must split/fragment the packet
//    virtual void SetMaxRoutablePayloadSize( int nSplitSize ) = 0; // 71
//    virtual int GetMaxRoutablePayloadSize() = 0;
//    virtual int GetProtocolVersion() = 0;
//    virtual void __unknown6( void ) = 0;
//    virtual void __unknown7( void ) = 0;
//    virtual void __unknown8( void ) = 0;
//    virtual void __unknown9( void ) = 0;
//    virtual void* GetChannelEncryptionKey( void ) = 0;
//    virtual void __unknown11( void ) = 0;
//
//    char pad_0000[0x20];            //0x0000 - Minus 8 for vtable.
//    bool m_bProcessingMessages;     //0x0028
//    bool m_bShouldDelete;           //0x0029
//    char pad_002A[2];               //0x002A
//    int m_nOutSequenceNr;       //0x002C
//    int m_nInSequenceNr;        //0x0030
//    int m_nOutSequenceNrAck;    //0x0034
//    int m_nOutReliableState;    //0x0038
//    int m_nInReliableState;     //0x003C
//    int m_nChokedPackets;       //0x0040
//    bf_write m_StreamReliable;      //0x0044
//    char pad_0068[12]; //0x0068
//    bf_write m_StreamUnreliable; //0x0074
//    char pad_0098[112]; //0x0098
//    char m_szServerIP[32]; //0x0108 This really probably is a netadr_t around here, but whatever
//    char pad_0128[36]; //0x0128
//    float m_flLastReceived; //0x014C Time last received
//    char pad_0150[4]; //0x0150
//    float m_flConnectTime; //0x0154
//    int m_nRate; //0x0158
//    char pad_015C[16768]; //0x015C
//    char N00000B08[32]; //0x42DC
//    char pad_42FC[4]; //0x42FC
//    float timeout; //0x4300 cl_timeout
//    char pad_4304[52]; //0x4304
//    float interpAmount; //0x4338
//    float serverFrameTime; //0x433C  Figured this out by doing the classic throw a grenade into 100 negevs on the ground.
//    float serverFrameTimestdDeviation; //0x4340 Also correlates to net_graph numbers.
//    float maxRoutablePayloadSize; //0x4344
//    int splitPacketSequence; //0x4348
//};

//class INetChannelHandler
//{
//public:
//    virtual    ~INetChannelHandler( void ) {};
//    virtual void ConnectionStart( INetChannel* chan ) = 0;    // called first time network channel is established
//    virtual void ConnectionClosing( const char* reason ) = 0; // network channel is being closed by remote site
//    virtual void ConnectionCrashed( const char* reason ) = 0; // network error occured
//    virtual void PacketStart( int incoming_sequence, int outgoing_acknowledged ) = 0;    // called each time a new packet arrived
//    virtual void PacketEnd( void ) = 0; // all messages has been parsed
//    virtual void FileRequested( const char* fileName, unsigned int transferID ) = 0; // other side request a file for download
//    virtual void FileReceived( const char* fileName, unsigned int transferID ) = 0; // we received a file
//    virtual void FileDenied( const char* fileName, unsigned int transferID ) = 0;    // a file request was denied by other side
//    virtual void FileSent( const char* fileName, unsigned int transferID ) = 0;    // we sent a file
//};

/// Unsure about these; ToString and Get/SetNetChannel should be ok
//class INetMessage
//{
//public:
//    virtual    ~INetMessage() {};
//    // Use these to setup who can hear whose voice.
//    // Pass in client indices (which are their ent indices - 1).
//    virtual void SetNetChannel( INetChannel* netchan ) = 0; // netchannel this message is from/for
//    virtual void SetReliable( bool state ) = 0;    // set to true if it's a reliable message
//    virtual bool Process( void ) = 0; // calles the recently set handler to process this message
//    virtual void __unknown1( void ) const = 0;
//    virtual void __unknown2( void ) const = 0;
//    virtual bool IsReliable( void ) const = 0;  // true, if message needs reliable handling
//    virtual int GetType( void ) const = 0; // returns module specific header tag eg svc_serverinfo
//    virtual int GetGroup( void ) const = 0;    // returns net message group of this message
//    virtual const char* GetName( void ) const = 0; // returns a human readable string about message content
//    virtual INetChannel* GetNetChannel( void ) const = 0;
//    virtual const char* ToString( void ) const = 0;
//    virtual void __unknown5( void ) const = 0;
//};

//class INetwork
//{
//public:
//    virtual ~INetwork( void ) {};
//    virtual void Init( void ) = 0;
//    virtual void Config( bool multiplayer );
//    virtual void IsMultiplayer( void ) = 0; // true = full MP mode, false = loopback SP mode
//    virtual void IsEnabled( void ) = 0;
//    // shuts down Network, closes all UPD/TCP channels
//    virtual void Shutdown( void ) = 0;
//    // must be called each system frame to do any asynchronouse TCP stuff
//    virtual void RunFrame( double time ) = 0;
//    virtual void LogBadPacket( netpacket_t* packet ) = 0;
//    // Address conversion
//    //    virtual bool StringToAdr ( const char *s, netadr_t *a) = 0;
//    // Convert from host to network byte ordering
//    virtual unsigned short HostToNetShort( unsigned short us_in );
//    // and vice versa
//    virtual unsigned short NetToHostShort( unsigned short us_in );
//};
