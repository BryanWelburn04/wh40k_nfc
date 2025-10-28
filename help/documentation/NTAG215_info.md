<INFORMATION OBTAIN FROM NXP.com - NTAG213_215_216 data sheet pdf>

# NTAG215 EEPROM: 540 bytes, in 135 pages, 4 byte per page
	- 26 bytes reserved for maufacturer and config data
	- 28 bits user for read-only locking mechanism
		\> ~locking mechanism?
	- 4 bytes available as capability container
		\> ~what does capability container mean?
	- 504 bytes user programmable read/write memory


# RF INTERFACE
	- Based on ISO/IEC 14443 Type A standard
	
	- NFC device generates RF field. Field must be present
	-	as it is used for communication and tag power supply
	
	- Dual direction data communication
	- 	one start bit at beginning of each frame.
		\> ~what is a frame??

	- Each byte is transmitted with an odd parity bit at the end
		
	- The LSB of the byte with lowest address of selected
	- 	block is transmitted first.

	- NFC device tag frame has a max length of 163 bits.
	- 	Max length of fixed size tag to NFC device frame is 307 bits
	- 	The FAST_READ command has a variable frame length
	-	Max frame length supported by NFC device is important.
		\> what is a frame?

	- Multi-byte parameter, LSB is always first. 
	Example: When reading memory using READ command,
		byte 0 from the addressed block is transmitted first,
		follow by bytes 1-3 out of this block.
		The same sequence continutes for all subsequent blocks.

# DATA INTEGRITY
	- Following mechanisms are implemented in the contactless
	- 	communication link between NFC device and NTAG
	[] 16 bits CRC per block
		\> what is CRC?
	[] parity bits for each byte
	[] bit count checking
	[] bit coding to distinguish "1", "0", "no info"
	[] channel monitoring (protocol sequence and bit stream analysis)

# IDLE STATE
	- After power-on reset (POR), NTAG215 switches to IDLE state

	- Only exits idle when REQA or WUPA command is received
	-	from NFC device

	- All other data received in this state is interpreted as an
	-	error and remains in IDLE state

# READY1 STATE
	- NFC device resolves first part of the UID (3 bytes) using
	-	ANTICOLLISION or SELECT commands in cascade level 1.
	
	- This state is correctly exited after execution of one of
	-	these following commands:
	[] SELECT: NFC device switches NTAG215 into READY2 state.
	[] READ command (from address 0): all anticollision mechanisms
		are bypassed and switches state directly to ACTIVE.
	\> keep in mind cases with mulitple NTAGS at once see data sheet

# READY2 state
	- Resolves second part of UID (4 bytes) with cascade level 2
	-	ANTICOLLISION command.

	- Usually exited with cascade level 2 SELECT command

	- Alternatively, READY2 can be skipped using READ command
	-	as described in READY1 state

	- Respond to cascasde level 2 SELECT command is the 
	- Select AcKnowledge (SAK) byte.
	-	this byte indicates if anticollision has finished.

	- NTAG215 is now uniquely selected and only this device will 
	-	communicate with the NFC device.

# ACTIVE STATE
	- All memory operations and other functions are operated
	-	in this state

	- Exited with the HLTA command and upon reception,
	-	NTAG215 transits to HALT state.

	- NTAG215 transits to AUTHENTICATED state after successful
	-	password verification using PWD_AUTH command.

# AUTHENTICATED STATE
	- ALL operations on memory pages, which are configured as
	-	password protected, can be accessed.

	- Exited with the HLTA command same as ACTIVE state is.

# HALT STATE
	- HALT and IDLE are the two wait states.

	- This state helps NFC device distinguish between processed
	-	tags and tags yet to be selected.

	- Exit this state only with the WUPA command.

# MEMORY ORGANIZATION
	- This section is ESSENTIAL
	-	Please look at https://www.nxp.com/docs/en/data-sheet/NTAG213_215_216.pdf
	- 	PAGE 11, Fig. 6

# UID/SERIAL NUMBER
	- Unique 7 byte UID and its 2 check bytes are in the first
	-	9 bytes of memory addressed at 00h, 01h, and first
	-	byte of page 02h. The second byte of 02h is also reserved.
	-	These bytes are programmed and write protected.

# STATIC LOCK BYTES
	- The bits of byte 2 & 3 of page 02h represent field programmable
	-	ROM locking mechanism. Page 03h (CC) to 0Fh can be
	-	individually locked by setting the corresponding locking
	-	bit Lx to logic 1 to prevent further write access.
	-	After locking, page becomes ROM.

	- 3 LSB of lock byte 0 are block-locking bits.
	-	Bit 2 deals with pages 0Ah to 0Fh,
	-	Bit 1 deals with pages 04h to 09h,
	-	Bit 0 deals with page 03h (CC).
	- Once block-locking bits are set, locking config for
	- 	memory area is frozen.

	ATTENTION!!! : The CC (capability container) page 3 is 
		ONE TIME PROGRAMMABLE (OTP). BE CAREFUL

# continue from here
# left off bottom of page 13
# DYNAMIC LOCK BYTES
