

#define KEYMAPPEDTOFILE_INSERT_OR_ASSIGN "insert_or_assign"
#define KEYMAPPEDTOFILE_CONTAINS_KEY "contains_key"
#define KEYMAPPEDTOFILE_AT "at"

//#define FOLDEROFFILES "forautomatedio"
//#define KEYSNFILENAMES_FILE "0"

//undone 
//

static void retAvailFilename(unsigned char *buf){
    //undone starting from 1
}


static int loadKeysnfilenames(void){
	unsigned char *keysnfilenames;
	size_t keysnfilenamesSi;
	READ_WHOLE_FILEs(keysnfilenames,keysnfilenamesSi,KEYSNFILENAMES_FILE,{
		LOGLITERALs("io failed")
		return RETVALFAILURE;
	},{
		LOGLITERALs("empty file, not ended with lf")
		return RETVALFAILURE;
	},{
		LOGLITERALs("malloc for reading metadata failed")
		return RETVALMEMALLOCFAIL;
	},);
    
    //undone keys and filenames are separated by \n
}

static int writeKeysnfilenamesToFile(void){
	//undone empty lines are allowed, so it's possible to delete records, and then put shorter records there, extra bytes are filled with \n

	//undone must be in the order of key memcmp (really necessary?)
	//all files are listed except 0
}

static int doesKeyExist(unsigned char *key){
    //undone
}

static int compareFileWith(unsigned char *key, unsigned char *dtocomparewith){
    //undone readFileWithKey and then do some comparison, maybe ignore white space? maybe add an arg like exactCompare/htmlCompare/textCompare?
}

static int readFileWithKey(unsigned char *key){
    //undone read the file, the file content is cached, 1 file is cached at most, when you read another file, previously read content is freed (bc you usually use a file's content later)
}

static int writeFileWithKey(unsigned char *key, unsigned char *content){
    //undone create/overwrite the file. As for KEYSNFILENAMES_FILE, you cache it
    //key cannot contain \n, empty content is allowed, in this case make empty file
}

static int deleteKeyNFile(unsigned char *key){
    //undone delete file immediately. As for KEYSNFILENAMES_FILE, you cache it
}
