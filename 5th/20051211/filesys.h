
#define BLOCKSIZ        512
#define SYSOPENFILE  40
#define DIRNUM 128
#define DIRSIZ 12    //xiao 14->12
#define PWDSIZ 12
#define PWDNUM 32
#define NOFILE   20
#define NADDR  10
#define NHINO 128       /* must be power of 2 */
#define USERNUM 10
#define DINODESIZ 52   //xiao 32->52


/*filesys*/
#define DINODEBLK  32
#define FILEBLK   512
#define NICFREE  50 
#define NICINOD  50
#define DINODESTART  (2*BLOCKSIZ)    /*1024   0x400*/
#define DATASTART  ((2+DINODEBLK)*BLOCKSIZ)     /*d:17408    0x4400*/


/*di_mode*/
#define DIEMPTY   00000
#define DIFILE      01000
#define DIDIR     02000

#define UDIREAD 00001          /* USER */
#define UDIWRITE  00002
#define UDIEXICUTE  00004
#define GDIREAD   00010  /* GROUP */
#define GDIWRITE  00020
#define GDIEXICUTE  00040
#define ODIREAD  00100   /* OTHER */
#define ODIWRITE 00200
#define ODIEXICUTE 00400


#define READ  1
#define WRITE 2
#define EXICUTE 3


#define DEFAULTMODE 00777


/* i_flag */
#define  IUPDATE  00002

/* s_fmod */
#define SUPDATE  00001 

/* f_flag */
#define FREAD   00001
#define FWRITE   00002
#define FAPPEND   00004

/* error */
#define DISKFULL  65535

/* fseek origin */
#define SEEK_SET  0


struct inode{
	struct inode  *i_forw;
	struct inode  *i_back;
	char i_flag;
	unsigned int  i_ino;          /*����i �ڵ��־*/
	unsigned int  i_count;     /*���ü���*/
	unsigned short  di_number; /*�����ļ�������Ϊ0 ʱ����ɾ�����ļ�*/
	unsigned short  di_mode;  /*��ȡȨ��*/
	unsigned short  di_uid;
	unsigned short  di_gid;
	unsigned short  di_size;   /*�ļ���С*/
	unsigned int   di_addr[NADDR];   /*������*/
};

struct dinode{
	unsigned short di_number; /*�����ļ���*/
	unsigned short di_mode; /*��ȡȨ��*/
	unsigned short di_uid;
	unsigned short di_gid;
	unsigned short di_size;  /*�ļ���С*/
	unsigned int di_addr[NADDR];   /*������*/
};


struct direct{
	char d_name[DIRSIZ];
	unsigned int d_ino;
	};

struct filsys{
	unsigned short  s_isize;   /*i�ڵ�����*/
	unsigned long   s_fsize;   /*���ݿ����*/
	unsigned int   s_nfree;    /*���п�*/
	unsigned short  s_pfree;  /*���п�ָ��*/
	unsigned int  s_free[NICFREE];  /*���п��ջ*/
	
	unsigned int  s_ninode;  /*number of free inode in s_inode*/
	unsigned short  s_pinode;  /*pointer of the sinode*/
	unsigned int  s_inode[NICINOD];   /*����i�ڵ�����*/
	unsigned int s_rinode;    /*remember inode*/

	char s_fmod;  /*�������޸ı�־*/
	};

struct pwd{
	unsigned short p_uid;
	unsigned short p_gid;
	char password [PWDSIZ];
	};

struct dir{
	struct direct direct[DIRNUM];
	int size;  /*��ǰĿ¼��С*/
	};

struct hinode{
	struct inode *i_forw;   /*HASG��ָ��*/
	};

struct file{
	char f_flag;    /*�ļ�������־*/
	unsigned int f_count;  /*���ü���*/
	struct inode *f_inode; /*ָ���ڴ�i�ڵ�*/
	unsigned long f_off;   /*read/write character pointer*/
	};

struct user{
	unsigned short u_default_mode;
	unsigned short u_uid;
	unsigned short u_gid;
	unsigned short u_ofile[NOFILE];   /*�û����ļ���*/
	
	};

extern struct hinode hinode[NHINO];
extern struct dir dir;  /*��ǰĿ¼(���ڴ���ȫ������)*/
extern struct file sys_ofile[SYSOPENFILE];
extern struct filsys filsys;  /*�ڴ��еĳ�����*/
extern struct pwd pwd[PWDNUM];
extern struct user user[USERNUM];
extern struct file  *fd; /*the file system column of all the system*/    //xiao

extern struct  inode *cur_path_inode;
extern int user_id;

extern char   disk[(DINODEBLK+FILEBLK+2)*BLOCKSIZ];

extern struct inode *iget();
extern iput();
extern unsigned int balloc();
extern bfree();
extern struct inode *ialloc();
extern ifree();
extern unsigned int namei();
extern unsigned short iname();
extern unsigned int access();
extern _dir();
extern mkdir();
extern chdir();
extern unsigned short open();
extern unsigned short creat();
extern unsigned int read();
extern unsigned int write();
extern int login();
extern logout();
extern install();
extern format();
extern close();
extern halt();



