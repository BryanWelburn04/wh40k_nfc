/**
 * @file
 * ACR API library header file.
 * @version 1.0
 * @date    17 Jan 2011
 * @warning Copyright (C) 2011 Advanced Card Systems Ltd. All rights reserved.
 */

/**
 * @mainpage
 *
 * @section Introduction Introduction
 *
 * This documentation covers the APIs provided by ACR API library. Currently,
 * the library supports ACR1281S reader.
 *
 * @section UsingACRAPI Using ACR API
 *
 * To use ACR API, your source code must include a header file "acr.h".
 * @code
 * #include <stdio.h>
 * #include <acr.h>
 *
 * int main(int argc, char *argv[])
 * {
 *     HANDLE hReader;
 *     DWORD ret;
 *
 *     // Open reader using COM1
 *     ret = ACR_Open(TEXT("\\\\.\\COM1"), &hReader);
 *     if (ret != ERROR_SUCCESS)
 *     {
 *         printf("ACR_Open failed with error 0x%08X\n", ret);
 *         return 1;
 *     }
 *
 *     // TODO: Place other API function calls here
 *     // ...
 *
 *     // Close reader
 *     ret = ACR_Close(hReader);
 *     if (ret != ERROR_SUCCESS)
 *         printf("ACR_Close failed with error 0x%08X\n", ret);
 *
 *     return 0;
 * }
 * @endcode
 *
 * @subsection OpeningReader Opening Reader
 *
 * On Windows, port name is specified as "\\.\COMx" where x is the port number.
 * e.g. "\\.\COM1" is the first serial port.
 *
 * On Linux, port name can be any file under directory "/dev". Stanard Linux
 * serial port driver creates the device file as "/dev/ttySx" where x is the
 * port number. e.g. "/dev/ttyS0" is the first serial port.
 *
 * ACR_Open() will return a handle of reader in parameter if the port is opened
 * successfully.
 *
 * @subsection ClosingReader Closing Reader
 *
 * After finishing the operation, use ACR_Close() to close the reader.
 *
 * @subsection UsingANSIandUnicodeFunctions Using ANSI and Unicode Functions
 *
 * If your source code defined <b>UNICODE</b> macro, the following functions
 * will be mapped to Unicode version.
 *
 * - ACR_Open() -> ACR_OpenW()
 *
 * Otherwise, it will be mapped to ANSI version.
 *
 * - ACR_Open() -> ACR_OpenA()
 */

#ifndef ACR_H
#define ACR_H

#include <windows.h>

// Error codes
#define ACR_ERROR_PROTO_MISMATCH        ((DWORD) 0xA010000FL)   ///< The requested protocols are incompatible with the protocol currently in use with the card. 
#define ACR_ERROR_UNSUPPORTED_CARD      ((DWORD) 0xA0100065L)   ///< The reader cannot communicate with the card, due to ATR string configuration conflicts. 
#define ACR_ERROR_UNRESPONSIVE_CARD     ((DWORD) 0xA0100066L)   ///< The smart card is not responding to a reset.  
#define ACR_ERROR_UNPOWERED_CARD        ((DWORD) 0xA0100067L)   ///< Power has been removed from the smart card, so that further communication is not possible. 
#define ACR_ERROR_REMOVED_CARD          ((DWORD) 0xA0100069L)   ///< The smart card has been removed, so further communication is not possible. 

// Protocol
#define ACR_PROTOCOL_UNDEFINED          0x00000000  ///< Protocol is undefined.
#define ACR_PROTOCOL_T0                 0x00000001  ///< T=0 protocol.
#define ACR_PROTOCOL_T1                 0x00000002  ///< T=1 protocol.

// ICC status
#define ACR_ICC_UNKNOWN                 0   ///< The current status is unknown. 
#define ACR_ICC_ABSENT                  1   ///< There is no card in the reader.                    
#define ACR_ICC_PRESENT                 2   ///< There is a card in the reader.
#define ACR_ICC_POWERED                 3   ///< Power is being provided to the card.

// Control codes
#define IOCTL_ACR_CCID_ESCAPE           3500    ///< Control code for sending CCID escape command.

// Timeouts
typedef struct _ACR_TIMEOUTS {
    /**
     * Status timeout in milliseconds. Default is 2000 ms.
     */
    DWORD statusTimeout;

    /**
     * Number of status retries. Default is 1.
     */
    DWORD numStatusRetries;

    /**
     * Response timeout in milliseconds. Default is 10000 ms.
     */
    DWORD responseTimeout;

    /**
     * Number of response retries. Default is 1.
     */
    DWORD numResponseRetries;
} ACR_TIMEOUTS, *PACR_TIMEOUTS;

/**
 * @struct _ACR_TIMEOUTS
 * Timeouts. This data structure is used in ACR_GetTimeouts() and
 * ACR_SetTimeouts() function.
 */

/**
 * @typedef ACR_TIMEOUTS
 * Create a type name for _ACR_TIMEOUTS.
 */

/**
 * @typedef PACR_TIMEOUTS
 * Create a type name for pointer to _ACR_TIMEOUTS data structure.
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup Reader
 */

/*@{*/

/**
 * Open reader (ANSI). The ACR_OpenA() function opens a reader and returns a
 * handle value as reference.
 * @param[in]  portName Port name.
 * <table>
 * <tr><td><b>Platform</b></td><td><b>Description</b></td></tr>
 * <tr><td>Windows</td><td>Port name is specified as "\\.\COMx" where x is the
 * port number. e.g. "\\.\COM1" is the first serial port.</td></tr>
 * <tr><td>Linux</td><td>Port name can be any file under directory "/dev".
 * Stanard Linux serial port driver creates the device file as "/dev/ttySx"
 * where x is the port number. e.g. "/dev/ttyS0" is the first serial port.</td>
 * </tr>
 * </table><br>
 * @param[out] phReader Pointer to the HANDLE variable.
 * @return Error code.
 * @retval ERROR_SUCCESS The operation completed successfully.
 * @retval Failure       An error code.
 * <table>
 * <tr><td><b>Platform</b></td><td><b>Description</b></td></tr>
 * <tr><td>Windows</td><td>See error codes from Windows API and ACR error codes.</td></tr>
 * <tr><td>Linux</td><td>See error codes from GNU C library header file "errno.h" and ACR error codes.</td></tr>
 * </table><br>
 * @see ACR_Close
 *
 * @code
 * HANDLE hReader;
 * DWORD ret;
 *
 * // Open reader using COM1
 * ret = ACR_Open(TEXT("\\\\.\\COM1"), &hReader);
 * if (ret != ERROR_SUCCESS)
 *     printf("ACR_Open failed with error 0x%08X\n", ret);
 * @endcode
 */
DWORD WINAPI ACR_OpenA(LPCSTR portName, LPHANDLE phReader);

/**
 * Open reader (Unicode). The ACR_OpenW() function opens a reader and returns a
 * handle value as reference.
 * @param[in]  portName Port name.
 * <table>
 * <tr><td><b>Platform</b></td><td><b>Description</b></td></tr>
 * <tr><td>Windows</td><td>Port name is specified as "\\.\COMx" where x is the
 * port number. e.g. "\\.\COM1" is the first serial port.</td></tr>
 * <tr><td>Linux</td><td>Port name can be any file under directory "/dev".
 * Stanard Linux serial port driver creates the device file as "/dev/ttySx"
 * where x is the port number. e.g. "/dev/ttyS0" is the first serial port.</td>
 * </tr>
 * </table><br>
 * @param[out] phReader Pointer to the HANDLE variable.
 * @return Error code.
 * @retval ERROR_SUCCESS The operation completed successfully.
 * @retval Failure       An error code.
 * <table>
 * <tr><td><b>Platform</b></td><td><b>Description</b></td></tr>
 * <tr><td>Windows</td><td>See error codes from Windows API and ACR error codes.</td></tr>
 * <tr><td>Linux</td><td>See error codes from GNU C library header file "errno.h" and ACR error codes.</td></tr>
 * </table><br>
 * @see ACR_Close
 *
 * @code
 * HANDLE hReader;
 * DWORD ret;
 *
 * // Open reader using COM1
 * ret = ACR_Open(TEXT("\\\\.\\COM1"), &hReader);
 * if (ret != ERROR_SUCCESS)
 *     printf("ACR_Open failed with error 0x%08X\n", ret);
 * @endcode
 */
DWORD WINAPI ACR_OpenW(LPCWSTR portName, LPHANDLE phReader);

/**
 * @def ACR_Open
 * ACR_Open will be mapped to ACR_OpenW() function if UNICODE is defined.
 * Otherwise, it will be mapped to ACR_OpenA() function.
 */
#ifdef UNICODE
#define ACR_Open    ACR_OpenW
#else
#define ACR_Open    ACR_OpenA
#endif

/**
 * Close reader. The ACR_Close() function closes the reader and releases the
 * resources.
 * @param[in] hReader A reference value returned from ACR_Open() function.
 * @return Error code.
 * @retval ERROR_SUCCESS The operation completed successfully.
 * @retval Failure       An error code.
 * <table>
 * <tr><td><b>Platform</b></td><td><b>Description</b></td></tr>
 * <tr><td>Windows</td><td>See error codes from Windows API and ACR error codes.</td></tr>
 * <tr><td>Linux</td><td>See error codes from GNU C library header file "errno.h" and ACR error codes.</td></tr>
 * </table><br>
 * @see ACR_Open
 *
 * @code
 * DWORD ret;
 *
 * // Close reader
 * ret = ACR_Close(hReader);
 * if (ret != ERROR_SUCCESS)
 *     printf("ACR_Close failed with error 0x%08X\n", ret);
 * @endcode
 */
DWORD WINAPI ACR_Close(HANDLE hReader);

/**
 * Get number of slots. The ACR_GetNumSlots() function retrieves the number
 * of slots in the reader.
 * @param[in]  hReader   A reference value returned from ACR_Open() function.
 * @param[out] pNumSlots Pointer to a DWORD variable in which the number of slots is returned.
 * @return Error code.
 * @retval ERROR_SUCCESS The operation completed successfully.
 * @retval Failure       An error code.
 * <table>
 * <tr><td><b>Platform</b></td><td><b>Description</b></td></tr>
 * <tr><td>Windows</td><td>See error codes from Windows API and ACR error codes.</td></tr>
 * <tr><td>Linux</td><td>See error codes from GNU C library header file "errno.h" and ACR error codes.</td></tr>
 * </table><br>
 * @see ACR_Open
 *
 * @code
 * DWORD numSlots;
 * DWORD ret;
 *
 * // Get number of slots
 * ret = ACR_GetNumSlots(hReader, &numSlots);
 * if (ret != ERROR_SUCCESS)
 *     printf("ACR_GetNumSlots failed with error 0x%08X\n", ret);
 * @endcode
 */ 
DWORD WINAPI ACR_GetNumSlots(HANDLE hReader, LPDWORD pNumSlots);

/**
 * Get baud rate. The ACR_GetBaudRate() function retrieves the baud rate of
 * reader.
 * @param[in]  hReader   A reference value returned from ACR_Open() function.
 * @param[out] pBaudRate Pointer to a DWORD variable in which the baud rate is returned.
 * @return Error code.
 * @retval ERROR_SUCCESS The operation completed successfully.
 * @retval Failure       An error code.
 * <table>
 * <tr><td><b>Platform</b></td><td><b>Description</b></td></tr>
 * <tr><td>Windows</td><td>See error codes from Windows API and ACR error codes.</td></tr>
 * <tr><td>Linux</td><td>See error codes from GNU C library header file "errno.h" and ACR error codes.</td></tr>
 * </table><br>
 * @see ACR_Open, ACR_SetBaudRate
 *
 * @code
 * DWORD baudRate;
 * DWORD ret;
 *
 * // Get baud rate
 * ret = ACR_GetBaudRate(hReader, &baudRate);
 * if (ret != ERROR_SUCCESS)
 *     printf("ACR_GetBaudRate failed with error 0x%08X\n", ret);
 * @endcode
 */
DWORD WINAPI ACR_GetBaudRate(HANDLE hReader, LPDWORD pBaudRate);

/**
 * Set baud rate. The ACR_SetBaudRate() function sets the baud rate of
 * reader. ACR1281S reader supports 9600, 19200, 38400, 57600, 115200, 128000,
 * 230400, 250000, 256000 and 500000 bps.
 * @param[in] hReader  A reference value returned from ACR_Open() function.
 * @param[in] baudRate Baud rate.
 * @return Error code.
 * @retval ERROR_SUCCESS The operation completed successfully.
 * @retval Failure       An error code.
 * <table>
 * <tr><td><b>Platform</b></td><td><b>Description</b></td></tr>
 * <tr><td>Windows</td><td>See error codes from Windows API and ACR error codes.</td></tr>
 * <tr><td>Linux</td><td>See error codes from GNU C library header file "errno.h" and ACR error codes.</td></tr>
 * </table><br>
 * @see ACR_Open, ACR_GetBaudRate
 *
 * @code
 * DWORD ret;
 *
 * // Set baud rate to 115200 bps
 * ret = ACR_SetBaudRate(hReader, 115200);
 * if (ret != ERROR_SUCCESS)
 *     printf("ACR_SetBaudRate failed with error 0x%08X\n", ret);
 * @endcode
 */
DWORD WINAPI ACR_SetBaudRate(HANDLE hReader, DWORD baudRate);

/**
 * Get timeouts. The ACR_GetTimeouts() function retrieves the timeout
 * parameters for status and response operations of the reader.
 * @param[in]  hReader   A reference value returned from ACR_Open() function.
 * @param[out] pTimeouts Pointer to a ACR_TIMEOUTS structure in which the timeout information is returned.
 * @return Error code.
 * @retval ERROR_SUCCESS The operation completed successfully.
 * @retval Failure       An error code.
 * <table>
 * <tr><td><b>Platform</b></td><td><b>Description</b></td></tr>
 * <tr><td>Windows</td><td>See error codes from Windows API and ACR error codes.</td></tr>
 * <tr><td>Linux</td><td>See error codes from GNU C library header file "errno.h" and ACR error codes.</td></tr>
 * </table><br>
 * @see ACR_Open, ACR_SetTimeouts
 *
 * @code
 * ACR_TIMEOUTS timeouts;
 * DWORD ret;
 *
 * // Get timeouts
 * ret = ACR_GetTimeouts(hReader, &timeouts);
 * if (ret != ERROR_SUCCESS)
 *     printf("ACR_GetTimeouts failed with error 0x%08X\n", ret);
 * @endcode
 */
DWORD WINAPI ACR_GetTimeouts(HANDLE hReader, PACR_TIMEOUTS pTimeouts);

/**
 * Set timeouts. The ACR_SetTimeouts() function sets the timeout parameters
 * for status and response operations of the reader.
 * @param[in] hReader   A reference value returned from ACR_Open() function.
 * @param[in] pTimeouts Pointer to a ACR_TIMEOUTS structure that contains the new timeout values.
 * @return Error code.
 * @retval ERROR_SUCCESS The operation completed successfully.
 * @retval Failure       An error code.
 * <table>
 * <tr><td><b>Platform</b></td><td><b>Description</b></td></tr>
 * <tr><td>Windows</td><td>See error codes from Windows API and ACR error codes.</td></tr>
 * <tr><td>Linux</td><td>See error codes from GNU C library header file "errno.h" and ACR error codes.</td></tr>
 * </table><br>
 * @see ACR_Open, ACR_GetTimeouts
 *
 * @code
 * ACR_TIMEOUTS timeouts;
 * DWORD ret;
 *
 * // Get timeouts
 * // ...
 *
 * // Modify status timeout to 100 ms
 * timeouts.statusTimeout = 100;
 *
 * // Set timeouts
 * ret = ACR_SetTimeouts(hReader, &timeouts);
 * if (ret != ERROR_SUCCESS)
 *     printf("ACR_SetTimeouts failed with error 0x%08X\n", ret);
 * @endcode
 */
DWORD WINAPI ACR_SetTimeouts(HANDLE hReader, const PACR_TIMEOUTS pTimeouts);

/*@}*/

/**
 * @addtogroup Card
 */

/*@{*/

/**
 * Power on ICC in slot. The ACR_PowerOnIcc function powers on the card in
 * the slot and returns the ATR string from the card.
 * @param[in]     hReader A reference value returned from ACR_Open() function.
 * @param[in]     slotNum Slot number.
 * @param[out]    atr     A pointer to the buffer that receives the ATR string returned from the card.
 * @param[in,out] pAtrLen The length in number of bytes of the atr parameter and receives the actual number of bytes received from the card.
 * @return Error code.
 * @retval ERROR_SUCCESS The operation completed successfully.
 * @retval Failure       An error code.
 * <table>
 * <tr><td><b>Platform</b></td><td><b>Description</b></td></tr>
 * <tr><td>Windows</td><td>See error codes from Windows API and ACR error codes.</td></tr>
 * <tr><td>Linux</td><td>See error codes from GNU C library header file "errno.h" and ACR error codes.</td></tr>
 * </table><br>
 * @see ACR_Open
 *
 * @code
 * BYTE atr[64];
 * DWORD atrLen = sizeof(atr);
 * DWORD ret;
 *
 * // Power on slot 0
 * ret = ACR_PowerOnIcc(hReader, 0, atr, &atrLen);
 * if (ret != ERROR_SUCCESS)
 *     printf("ACR_PowerOnIcc failed with error 0x%08X\n", ret);
 * @endcode
 */
DWORD WINAPI ACR_PowerOnIcc(HANDLE hReader, DWORD slotNum, LPBYTE atr, LPDWORD pAtrLen);

/**
 * Power off ICC in slot. The ACR_PowerOffIcc() function powers off the card
 * in the slot.
 * @param[in] hReader A reference value returned from ACR_Open() function.
 * @param[in] slotNum Slot number.
 * @return Error code.
 * @retval ERROR_SUCCESS The operation completed successfully.
 * @retval Failure       An error code.
 * <table>
 * <tr><td><b>Platform</b></td><td><b>Description</b></td></tr>
 * <tr><td>Windows</td><td>See error codes from Windows API and ACR error codes.</td></tr>
 * <tr><td>Linux</td><td>See error codes from GNU C library header file "errno.h" and ACR error codes.</td></tr>
 * </table><br>
 * @see ACR_Open
 *
 * @code
 * DWORD ret;
 *
 * // Power off slot 0
 * ret = ACR_PowerOffIcc(hReader, 0);
 * if (ret != ERROR_SUCCESS)
 *     printf("ACR_PowerOffIcc failed with error 0x%08X\n", ret);
 * @endcode
 */
DWORD WINAPI ACR_PowerOffIcc(HANDLE hReader, DWORD slotNum);

/**
 * Set protocol. The ACR_SetProtocol() function select the protocol and
 * parameters according to ATR string returned from the card. Before setting the
 * protocol, the card must be powered using ACR_PowerOnIcc() function.  
 * @param[in] hReader A reference value returned from ACR_Open() function.
 * @param[in] slotNum Slot number.
 * @param[in] preferredProtocols A bitmask of preferred protocols. Possible values may be combined with the OR operation.
 * <table>
 * <tr><td><b>Value</b></td><td><b>Meaning</b></td></tr>
 * <tr><td>ACR_PROTOCOL_T0</td><td>T=0 is an acceptable protocol.</td></tr>
 * <tr><td>ACR_PROTOCOL_T1</td><td>T=1 is an acceptable protocol.</td></tr>
 * </table><br>
 * @param[out] pActiveProtocol A flag that indicates the established active protocol.
 * <table>
 * <tr><td><b>Value</b></td><td><b>Meaning</b></td></tr>
 * <tr><td>ACR_PROTOCOL_T0</td><td>T=0 is the active protocol.</td></tr>
 * <tr><td>ACR_PROTOCOL_T1</td><td>T=1 is the active protocol.</td></tr>
 * <tr><td>ACR_PROTOCOL_UNDEFINED</td><td>Protocol is undefined.</td></tr>  
 * </table><br> 
 * @return Error code.
 * @retval ERROR_SUCCESS The operation completed successfully.
 * @retval Failure       An error code.
 * <table>
 * <tr><td><b>Platform</b></td><td><b>Description</b></td></tr>
 * <tr><td>Windows</td><td>See error codes from Windows API and ACR error codes.</td></tr>
 * <tr><td>Linux</td><td>See error codes from GNU C library header file "errno.h" and ACR error codes.</td></tr>
 * </table><br>
 * @see ACR_Open, ACR_PowerOnIcc
 *
 * @code
 * DWORD activeProtocol; 
 * DWORD ret;
 *  
 * // Power on slot 0
 * // ... 
 *
 * // Set protocol to T=0 or T=1
 * ret = ACR_SetProtocol(hReader, 0, ACR_PROTOCOL_T0 | ACR_PROTOCOL_T1, &activeProtocol);
 * if (ret != ERROR_SUCCESS)
 *     printf("ACR_SetProtocol failed with error 0x%08X\n", ret);
 * @endcode
 */
DWORD WINAPI ACR_SetProtocol(HANDLE hReader, DWORD slotNum, DWORD preferredProtocols, LPDWORD pActiveProtocol);

/**
 * Get ICC status. The ACR_GetIccStatus() function returns the current ICC
 * status in the slot of reader.   
 * @param[in]  hReader    A reference value returned from ACR_Open() function.
 * @param[in]  slotNum    Slot number.
 * @param[out] pIccStatus Pointer to a DWORD variable in which the ICC status is returned.
 * <table>
 * <tr><td><b>Value</b></td><td><b>Meaning</b></td></tr>
 * <tr><td>ACR_ICC_UNKNOWN</td><td>The current status is unknown.</td></tr> 
 * <tr><td>ACR_ICC_ABSENT</td><td>There is no card in the reader.</td></tr>                    
 * <tr><td>ACR_ICC_PRESENT</td><td>There is a card in the reader.</td></tr>
 * <tr><td>ACR_ICC_POWERED</td><td>Power is being provided to the card.</td></tr>
 * </table><br>
 * @return Error code.
 * @retval ERROR_SUCCESS The operation completed successfully.
 * @retval Failure       An error code.
 * <table>
 * <tr><td><b>Platform</b></td><td><b>Description</b></td></tr>
 * <tr><td>Windows</td><td>See error codes from Windows API and ACR error codes.</td></tr>
 * <tr><td>Linux</td><td>See error codes from GNU C library header file "errno.h" and ACR error codes.</td></tr>
 * </table><br>
 * @see ACR_Open
 *
 * @code
 * DWORD iccStatus; 
 * DWORD ret;
 *
 * // Get ICC status in slot 0
 * ret = ACR_GetIccStatus(hReader, 0, &iccStatus);
 * if (ret != ERROR_SUCCESS)
 *     printf("ACR_GetIccStatus failed with error 0x%08X\n", ret);
 * @endcode
 */
DWORD WINAPI ACR_GetIccStatus(HANDLE hReader, DWORD slotNum, LPDWORD pIccStatus);

/**
 * Exchange APDU. The ACR_ExchangeApdu() function sends APDU command and
 * receives APDU response from the card.
 * @param[in]     hReader        A reference value returned from ACR_Open() function.
 * @param[in]     slotNum        Slot number.
 * @param[in]     sendBuffer     A pointer to the actual data to be written to the card.
 * @param[in]     sendBufferLen  The length in number of bytes of the sendBuffer parameter.
 * @param[out]    recvBuffer     A pointer to any data returned from the card.
 * @param[in,out] pRecvBufferLen The length in number of bytes of the recvBuffer parameter and receives the actual number of bytes received from the card.
 * @return Error code.
 * @retval ERROR_SUCCESS The operation completed successfully.
 * @retval Failure       An error code.
 * <table>
 * <tr><td><b>Platform</b></td><td><b>Description</b></td></tr>
 * <tr><td>Windows</td><td>See error codes from Windows API and ACR error codes.</td></tr>
 * <tr><td>Linux</td><td>See error codes from GNU C library header file "errno.h" and ACR error codes.</td></tr>
 * </table><br>
 * @see ACR_Open
 *
 * @code
 * BYTE command[] = { 0x80, 0x84, 0x00, 0x00, 0x08 };
 * DWORD commandLen = sizeof(command);
 * BYTE response[300];
 * DWORD responseLen = sizeof(response);
 * DWORD ret;
 *
 * // Exchange APDU in slot 0
 * ret = ACR_ExchangeApdu(hReader, 0, command, commandLen, response, &responseLen);
 * if (ret != ERROR_SUCCESS)
 *     printf("ACR_ExchangeApdu failed with error 0x%08X\n", ret);
 * @endcode
 */
DWORD WINAPI ACR_ExchangeApdu(HANDLE hReader, DWORD slotNum, const LPBYTE sendBuffer, DWORD sendBufferLen, LPBYTE recvBuffer, LPDWORD pRecvBufferLen);

/*@}*/

/**
 * @addtogroup Peripheral
 */

/*@{*/
 
/**
 * Control peripheral. The ACR_Control() function gives you direct control
 * peripheral in the reader.
 * @param[in]  hReader        A reference value returned from ACR_Open() function.
 * @param[in]  slotNum        Slot number.
 * @param[in]  controlCode    Control code for the operation. This value identifies the specific operation to be performed.
 * @param[in]  inBuffer       Pointer to a buffer that contains the data required to perform the operation. This parameter can be NULL if the controlCode parameter specifies an operation that does not require input data.
 * @param[in]  inBufferSize   Size, in bytes, of the buffer pointed to by inBuffer.
 * @param[out] outBuffer      Pointer to a buffer that receives the operation's output data. This parameter can be NULL if the controlCode parameter specifies an operation that does not produce output data.
 * @param[in]  outBufferSize  Size, in bytes, of the buffer pointed to by outBuffer.      
 * @param[out] pBytesReturned Pointer to a DWORD that receives the size, in bytes, of the data stored into the buffer pointed to by outBuffer.
 * @return Error code.
 * @retval ERROR_SUCCESS The operation completed successfully.
 * @retval Failure       An error code.
 * <table>
 * <tr><td><b>Platform</b></td><td><b>Description</b></td></tr>
 * <tr><td>Windows</td><td>See error codes from Windows API and ACR error codes.</td></tr>
 * <tr><td>Linux</td><td>See error codes from GNU C library header file "errno.h" and ACR error codes.</td></tr>
 * </table><br>
 * @see ACR_Open
 *
 * @code
 * BYTE command[] = { 0xE0, 0x00, 0x00, 0x18, 0x00 };
 * DWORD commandLen = sizeof(command);
 * BYTE response[20];
 * DWORD responseLen = sizeof(response);
 * DWORD ret;
 *
 * // Get firmware version
 * ret = ACR_Control(hReader, 0, IOCTL_ACR_CCID_ESCAPE, command, commandLen, response, responseLen, &responseLen);
 * if (ret != ERROR_SUCCESS)
 *     printf("ACR_Control failed with error 0x%08X\n", ret);
 * @endcode
 */
DWORD WINAPI ACR_Control(HANDLE hReader, DWORD slotNum, DWORD controlCode, LPCVOID inBuffer, DWORD inBufferSize, LPVOID outBuffer, DWORD outBufferSize, LPDWORD pBytesReturned);

/*@}*/

#ifdef __cplusplus
}
#endif

#endif
