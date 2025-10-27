/////////////////////////////////////////////////////////////////////////////
// Reference.h : header file
/////////////////////////////////////////////////////////////////////////////

#pragma once

enum ACTION_RECORD
{
	DO_THE_ACTION = 0x00,
	SAVE_FOR_LATER = 0x01,
	OPEN_FOR_EDITING = 0x02,
	NO_ACTION
};

enum CONNECTION_SPEED
{
	KBPS_106 = 0x01,
	KBPS_212 = 0x02,
	KBPS_424 = 0x03,
	UNKNOWN_SPEED = 0x00
};

enum NFC_MODE
{
    PEER_TO_PEER_INITIATOR = 0x08,
    INITIATOR_DEMO = 0x06,
    PEER_TO_PEER_TARGET = 0x04,
    CARD_READER_WRITER = 0x00,
    MIFARE_ULTRALIGHT_CARD_EMULATION = 0x01,
    FELICA_CARD_EMULATION = 0x03
};

enum PROTOCOL_18092
{
	ATR_RES = 0x01, // Attribute Request CMD2 Byte
	WUP_RES = 0x03, // Wakeup Request CMD2 Byte
	PSL_RES = 0x05, // Parameter Selection Request CMD2 Byte
	DEP_RES = 0x07, // Data Exchange Protocol Request CMD2 Byte
	DSL_RES = 0x09, // Deselect Request CMD2 Byte
	RLS_RES = 0x0B, // Release Request CMD2 Byte
	ATR_REQ = 0x00, // Attribute Response CMD2 Byte
	WUP_REQ = 0x02, // Wakeup Response CMD2 Byte
	PSL_REQ = 0x04, // Parameter Selection Response CMD2 Byte
	DEP_REQ = 0x06, // Data Exchange Protocol Response CMD2 Byte
	DSL_REQ = 0x08, // Deselect Response CMD2 Byte
	RLS_REQ = 0x0A  // Release Response CMD2 Byte
};

enum CONNECTION_STATUS
{ 
	RECEIVE_DIAL = 0x00,
	RECEIVE_CONNECTED = 0x01,
	RECEIVE_FINISHED = 0x02,
	RECEIVE_DISCONNECTED = 0x03
};

enum LLC_TYPE
{ 
	SYMMETRY = 0x00,
	PARAMETER_EXCHANGE = 0x01,
	AGGREGATED_FRAME = 0x02,
	UNNUMBERED_INFORMATION = 0x03,
	CONNECT = 0x04,
	DISCONNECT = 0x05,
	CONNECTION_COMPLETE = 0x06,
	DISCONNECTED_MODE = 0x07,
	FRAME_REJECT = 0x08,
	SERVICE_NAME_LOOKUP = 0x09,
	INFORMATION = 0x0C,
	RECEIVE_READY = 0x0D,
	RECEIVE_NOT_READY = 0x0E
};

enum OPERATION_MODE
{
	ACTIVE = 0x01,
	PASSIVE = 0x02,
	UNKNOWN_MODE = 0x00
};

enum TYPE_NAME_FORMAT
{
    EMPTY = 0x00,
    NFC_FORM_WELL_KNOWN_TYPE = 0x01,
    MEDIATYPE = 0x02,
    ABSOLUTE_URI = 0x03,
    NFC_FORM_EXTERNAL_TYPE = 0x04,
    UNKNOWN = 0x05
};

enum URI_IDENTIFIER_CODE
{
    NONE = 0x00,
    HTTP_WWW = 0x01,
    HTTPS_WWW = 0x02,
    HTTP = 0x03,
    HTTPS = 0x04,
    TEL = 0x05,
    MAIL_TO = 0x06,
    FTP_ANONYMOUS = 0x07,
    FTP_FTP = 0x08,
    FTPS = 0x09,
    SFTP = 0x0A,
    SMB = 0x0B,
    NFS = 0x0C,
    FTP = 0x0D,
    DAV = 0x0E,
    NEWS = 0x0F,
    TELNET = 0x10,
    IMAP = 0x11,
    RTSP = 0x12,
    URN = 0x13,
    POP = 0x14,
    SIP = 0x15,
    SIPS = 0x16,
    TFTP = 0x17,
    BTSPP = 0x18,
    BT12CAP = 0x19,
    BTGOEP = 0x1A,
    TCPOBEX = 0x1B,
    IRDAOBEX = 0x1C,
    URI_FILE = 0x1D,
    URN_EPC_ID = 0x1E,
    URN_EPC_TAG = 0x1F,
    URN_EPC_PAT = 0x20,
    URN_EPC_RAW = 0x21,
    URN_EPC = 0x22,
	URN_NFC = 0x23
};

enum TAGS
{
	NULL_TLV = 0x00,
	NDEF_MESSAGE_TLV = 0x03,
	PROPRIETARY_TLV = 0xFD,
	TERMINATOR_TLV = 0xFE
};

enum ADDRESS_TYPE
{
	DOMESTIC = 0x01,
    INTERNATIONAL = 0x02,
    POSTAL = 0x04,
    PARCEL = 0x08,
    ADD_HOME = 0x10,
    ADD_WORK = 0x20,
    ADD_PREFERRED = 0x40
};

enum TEL_NUMBER_TYPE
{
    /// <summary>
    /// home telephone number
    /// </summary>
    TEL_HOME = 0x01,

    /// <summary>
    /// telephone number has voice messaging support
    /// </summary>
    TEL_MSG = 0x02,

    /// <summary>
    /// Work telephone number
    /// </summary>
    TEL_WORK = 0x03,

    /// <summary>
    /// preferred-use telephone number
    /// </summary>
    TEL_PREFERRED = 0x04,

    /// <summary>
    /// voice telephone number
    /// </summary>
    VOICE = 0x05,

    /// <summary>
    /// facsimile telephone number
    /// </summary>
    FAX = 0x06,

    /// <summary>
    /// cellular telephone number
    /// </summary>
    CELL = 0x07,

    /// <summary>
    /// video conferencing telephone number
    /// </summary>
    VIDEO = 0x08,

    /// <summary>
    /// paging device telephone number
    /// </summary>
    PAGER = 0x09,

    /// <summary>
    /// bulletin board system telephone number
    /// </summary>
    BBS = 0x0A,

    /// <summary>
    /// MODEM connected telephone number
    /// </summary>
    MODEM = 0x0B,

    /// <summary>
    /// car-phone telephone number
    /// </summary>
    CAR = 0x0C,

    /// <summary>
    /// ISDN service telephone number
    /// </summary>
    ISDN = 0x0D,

    /// <summary>
    /// personal communication services 
    /// </summary>
    PCS = 0x0E
};

enum EMAIL_TYPE
{
    INTERNET = 0x01,
    x400 = 0x02,
    EMAIL_PREFERRED = 0x04
};