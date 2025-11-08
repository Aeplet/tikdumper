#define DUMP_FOLDER "/tikdumper"

#define TITLE_UPPER(x)		((u32)((x) >> 32))
#define TITLE_LOWER(x)		((u32)(x))
#define TITLE_ID(x,y)		(((u64)(x) << 32) | (y))

#define MAX_IOS_ERROR_MESSAGE_SIZE 100

#define MEM_PROT                    (MEM_REG_BASE + 0x20a)
#define MEM_REG_BASE                0xd8b4000

#define AHBPROT_DISABLED			(*(vu32*)0xcd800064 == 0xFFFFFFFF)

#define TICKET_SIZE 0x2A4
#define TICKET_ALLOC (TICKET_SIZE + 0x20)

typedef enum {
    SD_CARD,
    USB_STORAGE,
    SDGECKO_A,
    SDGECKO_B,
} storage_device_t;