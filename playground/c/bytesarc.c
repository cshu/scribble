#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

struct CArr {
    char *ch;
    long length;
};

struct ColExt {
    struct ColExt *next;
    unsigned char col[4];
    unsigned char sub[4];
};

int ColExtFree(struct ColExt *ce) {
    if (ce->next != NULL)
        ColExtFree(ce->next);
    free(ce);
}

char *BSearchForC(char key, struct CArr base) {
    for (long i = 0; i < base.length; i++) {
        if (base.ch[i] == key) {
            return base.ch + i;
        }
    }
    return NULL;
}

int ArchiveBytes(struct CArr buffers[], unsigned numOfFiles, struct ColExt **outce) {
    //get an auto available LISTID,
    unsigned char listID[4];
    listID[0] = 255;
    listID[1] = 255;
    listID[2] = 255;
    listID[3] = 255;

    struct ColExt *pce;

    int isFirst = 1;

    for (unsigned i = 0; i < numOfFiles; i++) {
        for (long j = 0; j < buffers[i].length; j++) {
            if (j + 1 == buffers[i].length) {
                pce->next = NULL;
                pce->sub[3] = buffers[i].ch[j];
            } else {
                struct ColExt *nce;
                nce = (struct ColExt *) malloc(sizeof *nce);
                if (isFirst) {
                    *outce = nce;
                } else {
                    pce->next = nce;
                    pce->sub[3] = buffers[i].ch[j];
                    isFirst = 0;
                }

                nce->sub[0] = '\0';
                nce->sub[1] = '\0';
                nce->sub[2] = buffers[i].ch[j];
                pce = nce;
            }
        }
    }

    //TODO after all lists are stored in ColExt, specify the each 

    return EXIT_SUCCESS;
}

int ReadAllBytes(char* filename, char** buffer, long* bufLen) {

    FILE *ifp;
    ifp = fopen(filename, "rb");

    if (!ifp) {
        printf("Unable to open file!");
        return EXIT_FAILURE;
    }

    //Get file length
    fseek(ifp, 0, SEEK_END);
    *bufLen = ftell(ifp);
    fseek(ifp, 0, SEEK_SET);

    //Allocate memory
    *buffer = (char *) malloc(*bufLen + 1); //TODO + 1 needed???
    if (!*buffer) {
        printf("Memory Error!");
        return EXIT_FAILURE;
    }

    fread(*buffer, *bufLen, 1, ifp);
    fclose(ifp);
    return EXIT_SUCCESS;
}

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    int i;
    for (i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

/*
 * db == NULL means ending current list (if there is), and ready for new list.
 */
unsigned AddByteToList(unsigned char byte, sqlite3 *db) {
    static unsigned char prev;
    static unsigned listid = 0; //1. as the list ID. 2. 0 means uninit new list
    static unsigned listidNextVal = 0; //like a database sequence
    //65535 or less are elementary lists
    //divided by 256 equals char A
    //mod 256 equals char B
    //A<B, condition of validity
    static char *sql = NULL;
    if (sql == NULL)
        sql = (char *) malloc((sizeof *sql)*100); //TODO: 100 set constant?
    int rc;
    char *zErrMsg = NULL;
    if (db == NULL) {
        unsigned tobereturned = listid;
        listid = 0;
        free(sql);
        return tobereturned;
    }
    if (listid == 0) {
        //TODO debug code
        //listid = 4294967295;

        listid = listidNextVal;
        listidNextVal++;

        return;
    } else {
        if (prev > byte)
            sprintf(sql, "INSERT INTO lists (col,sub) VALUES (%u,%u);", listid, byte * 256 + prev);
        else
            sprintf(sql, "INSERT INTO lists (col,sub) VALUES (%u,%u);", listid, prev * 256 + byte);
        printf("%u\n%u\n%s\n", prev, byte, sql);
        rc = sqlite3_exec(db, sql, NULL, NULL, &zErrMsg);
        if (rc != SQLITE_OK) {
            printf("%s\n", zErrMsg);
        }
    }
    prev = byte;
}

/*
 * 
 */
int main(int argc, char** argv) {
    //TODO: check if a dir/file, Get the list of files
    unsigned numOfFiles = 2;
    char *filenames[numOfFiles];
    unsigned listids[numOfFiles]; //store the list ID

    filenames[0] = "D:\\SourceCodeOrScriptArchives\\PLAYGROUND\\BytesArchiver\\testdata.txt";
    filenames[1] = "D:\\SourceCodeOrScriptArchives\\PLAYGROUND\\BytesArchiver\\testdata.txt";

    //TODO: get the db filename, warning if the file exists, delete old file option?
    char *dbfilename = "D:\\SourceCodeOrScriptArchives\\PLAYGROUND\\BytesArchiver\\testdata.DB";
    sqlite3 *db;
    int rc;
    rc = sqlite3_open(dbfilename, &db);
    if (rc == SQLITE_OK) {
        printf("Database opening success.\n");
    } else {
        printf("Database opening failure.\n");
    }
    char *sql;
    char *zErrMsg = NULL;
    sql = "CREATE TABLE lists(col INT NOT NULL, sub INT NOT NULL, PRIMARY KEY (col, sub));";
    rc = sqlite3_exec(db, sql, NULL, NULL, &zErrMsg);
    if (rc == SQLITE_OK) {
        printf("Table creation success.\n");
    } else {
        printf("Table creation failure.\n");
    }

    unsigned char *buffer;
    buffer = (unsigned char *) malloc(sizeof *buffer);
    FILE *ifp;
    for (unsigned fileIndex = 0; fileIndex < numOfFiles; fileIndex++) {

        ifp = fopen(filenames[fileIndex], "rb");
        if (!ifp) {
            printf("Unable to open file!");
            return EXIT_FAILURE;
        }
        //TODO: add a counter to the loop, if size is not right, it's a reading error.
        while (fread(buffer, 1, 1, ifp) == 1) {
            unsigned li = AddByteToList(*buffer, db);

            if (li == 4294967295) {
                printf("Max integer 4294967295 reached!");
                //TODO exit?
            }
        }
        fclose(ifp);
        listids[fileIndex] = AddByteToList('\0', NULL);
    }
    free(buffer);
    //TODO: filenames and listids are stored into a testdata.TABLE file.
    sqlite3_close(db);

    return EXIT_SUCCESS;














    struct CArr buffers[numOfFiles];

    for (unsigned fileIndex = 0; fileIndex < numOfFiles; fileIndex++) {
        buffers[fileIndex].ch = NULL;
        ReadAllBytes(filenames[fileIndex], &(buffers[fileIndex].ch), &(buffers[fileIndex].length));
        buffers[fileIndex].ch[buffers[fileIndex].length] = '\0';
        //printf("%d\n", fileLenArr[fileIndex]);
        //    for (int i = 0; i <= fileLen; i++)
        //        printf("%c", buffer[i]);
    }
    struct ColExt *outce;
    ArchiveBytes(buffers, numOfFiles, &outce);

    for (unsigned fileIndex = 0; fileIndex < numOfFiles; fileIndex++) {
        free(buffers[fileIndex].ch);
    }

    //    FILE * pFile;
    //    pFile = fopen("D:\\SourceCodeOrScriptArchives\\PLAYGROUND\\BytesArchiver\\myfile.txt", "wb");
    //    fwrite(buffer, fileLen, 1, pFile);
    //    fclose(pFile);

    ColExtFree(outce);

    return (EXIT_SUCCESS);
}

