import java.util.*;
import java.lang.*;
import java.io.*;

class JOO
{
public static void main (String[] args) throws Exception
{
String fullFilename="D:\\_Registered\\compi`~!@#$%我们^&()_-=+[]{};',.le.txt";
InputStream inpStrm = Runtime.getRuntime().exec(new String[]{"cmd","/c @echo off & for %I in (\""+fullFilename+"\") do a.exe %~fsI"}).getInputStream();
long fileTimeUtc=0;
		 for (;;) {
			 int incomingByte = inpStrm.read();
			 if (incomingByte < 48 || incomingByte > 57) break;
			fileTimeUtc = fileTimeUtc * 10 + incomingByte - 48;
		 }//negative is not handled here
		 System.out.println(fileTimeUtc);
return;


}
}

