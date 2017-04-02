package griddoor.iiSwBasedSpInterfaces;

import griddoor.IIPropSets.BytesSense;
import griddoor.SwingBasedEditorUI.EditorInterfaceJFrame;
import griddoor.util.ArrayListTable;
import griddoor.util.StringPrepender;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.logging.Logger;

public class Globals {
	
	public static String defaultStorageLoca;
	public static String defaultRegisteredLoca;
	public static String defaultRegisteredLocSub;
	public static String exLogFullname;
	
	public static final int ANIMATION_mid = 0;
	public static final int ANIMATION_title = 1;
	public static final int ANIMATION_english = 2;
	public static final int ANIMATION_synonyms = 3;
	public static final int ANIMATION_episodes = 4;
	public static final int ANIMATION_type = 5;
	public static final int ANIMATION_ess = 6;//essential text
	
	public static final int FILE_snm = 0;
	public static final int FILE_onm = 1;
	public static final int FILE_size = 0;
	public static final int FILE_wtime = 1;
	
	public static final int SHORTNOTE_text = 0;
	
	public static Logger getLogger(){
		return griddoor.SwingBasedEditorUI.Globals.getLogger();
	}
//	public static void makeLineAbbr(StringBuilder sb, String line){
//		griddoor.SwingBasedEditorUI.Globals.makeLineAbbr(sb, line);
//	}
	public static void makeLineAbbr(StringPrepender sp, String line){
		griddoor.SwingBasedEditorUI.Globals.makeLineAbbr(sp, line);
	}
//	public static void makeLeadingCharsForSelected(StringBuilder sb){
//		griddoor.SwingBasedEditorUI.Globals.makeLeadingCharsForSelected(sb);
//	}
//	public static void makeLeadingCharsForNotSelected(StringBuilder sb){
//		griddoor.SwingBasedEditorUI.Globals.makeLeadingCharsForNotSelected(sb);
//	}

	static Socket clientSocket;
	private static OutputStream outpStreSo;
	private static InputStream inpuStreSo;
	public static OutputStream getOuStre() throws UnknownHostException, IOException{
		initSocketIfNull();
		return outpStreSo;
	}
	public static InputStream getInStre() throws UnknownHostException, IOException{
		initSocketIfNull();
		return inpuStreSo;
	}
	static EditorInterfaceJFrame mainJfr;
	public static String hostNameOrAddr;
	private static void initSocketIfNull() throws UnknownHostException, IOException{
		if(clientSocket==null){
			clientSocket = new Socket(hostNameOrAddr, 28420);//undone load host and port from config file
			outpStreSo = clientSocket.getOutputStream();
			inpuStreSo = clientSocket.getInputStream();
			//undone get the endianness/format of server 64 bit integer!
			mainJfr.staBarStr = "CONNECTED";
		}
	}
	public static void closSocketIfNotNull() throws IOException{
		if(clientSocket==null)
			return;
		clientSocket.shutdownOutput();
		int lastread = inpuStreSo.read();//?reading the last -1 FIN is necessary for the TCP four-way handshake termination? not sure, especially for java socket, might be different from Berkeley
		clientSocket.shutdownInput();
		outpStreSo.close();
		inpuStreSo.close();
		clientSocket.close();
		clientSocket = null;
		mainJfr.staBarStr = "Disconnected";
		if(lastread!=-1)
			throw new IOException("Last read value is not -1 (FIN)");
	}
	
	public static String getErrorMsgFromCod(int cod){
		switch(cod){
		case 3:
			return cod+"`RETVALMEMALLOCFAIL";
		case 4:
			return cod+"`RETVALINVALIDSTATE";
		case 252:
			return cod+"`RETVALTAUTOLOGY";
		case 253:
			return cod+"`RETVALOXYMORON";
		case 254:
			return cod+"`RETVALNOTFOUND";
		default:
			return cod+"`Unknown error code";
		}
	}
	
	public static final int RESERV = 0;
	public static final int SEARCH = 1;
	public static final int SUBMIT = 2;
	public static final int NEW_V = 3;
	public static final int SEARCH_P = 4;
	public static final int NEW_P = 5;
	
	
//	public static final int DELETE_SAPROP = 184;
//	public static final int REPLACE_SAPROP = 185;
	public static final int URLBOOKMARK_VSEARCH = 162;
	public static final int SELECTVERBSTHATDONOTENTAIL = 163;
	public static final int CHKPROPSVALIDITY = 164;
	public static final int DELETE_MUTEX = 165;
	public static final int UPSERT_MUTEX = 166;
	public static final int SELECT_MUTEXGROUP = 167;
	public static final int SELECTMUTEXCONTAININGVERB = 168;
	public static final int INSERT_ENTAILMENT = 169;
	public static final int DELETE_ENTAILMENT = 170;
	public static final int SELECTVERBSDIRECTLYENTAILEDBYTHIS = 171;
	public static final int SELECTVERBSDIRECTLYENTAILINGTHIS = 172;
	public static final int DELETE_SAVERB = 173;
	public static final int SHORTNOTES_VSEARCH = 174;
	public static final int UPSERT_URLBOOKMARK = 175;
	public static final int SELECT_URLBOOKMARK = 176;
	public static final int DELETE_URLBOOKMARK = 177;
	public static final int SEARCH_URLBOOKMARK = 178;
	public static final int UPSERTORDELETE_WORDSENSEGROUP = 179;
	public static final int SELECT_ESSENTIALTEXT = 180;
	public static final int SEARCH_WORDSENSEGROUP = 181;
	public static final int SELECT_SAVERB = 182;
	public static final int UPSERT_SAVERB = 183;
	public static final int DELETE_ANIMATION = 184;
	public static final int UPSERT_ANIMATION = 185;
	public static final int SELECT_SAPROP = 186;
	public static final int APPLICABLEGENERALVERBS = 187;
	public static final int SAVESAPROPS = 188;
	public static final int SEARCH_VERBDESC = 189;
	public static final int DELETE_SHORTNOTE = 190;
	public static final int SELECT_SHORTNOTE = 191;
	public static final int SEARCH_SHORTNOTETEXT = 192;
	public static final int UPSERT_SHORTNOTETEXTONLY = 193;
	public static final int DELETE_FROMFILESTNDATASTINFO = 194;
	public static final int INSERT_DATASTOREINFO = 195;
	public static final int INSERT_FILEALLOCNAME = 196;
	public static final int UPSERT_MALANIME = 197;
	public static final int CHKANIMEREGISTRATION = 198;
	public static final int SELECT_ANIME = 199;
	public static final int SEARCH_ANIMEWITHFILE = 200;
	//public static final int//undone exec arbitrary sql? 
	
	
	public static final int SELECTV  = 255;
	
	//undone including MAL entries cached here
	
	//undone including various senses/info/states cached here
	
	//undone and tabs??
	
	//undone write a class named BasePropSet, which is meant to be the base of lots of classes about prop data
	//e.g. MALEntry, Url, Audio product, DigitalFile, each object contains byte array data for creating props (1 or multiple)
	//applying these objects, you can send requests to server.
	//sometime you relates 2 objects of different class, like DigitalFile with MalEntry, to create some prop
    //Also, props can be selected from db, sent from iidb to client, relating these props with BasePropSets, you can make new props
    
	
	//undone you send the content of BasePropSets to server, and then the server updates db
	//each time you send it, it's like an atomic operation, e.g. you send multiple props, which are actually stored in the same table
	//so does server insert a row and then update the row? or the server just insert once?
	
	
	
	//so find more examples (in your past SNL notes) to determine further designs

	
	
	
	
	
	
	
	//undone you add an in-memory db here, it's an abstract iidb replica
	//but it's only partial for the client's use, since you cannot send request to server each time you update your ui
	//that is to say: our UI does not show the up-to-date data in iidb unless you explicitly press a button like "refresh" / "search"
	
	//note in your client, you keep a SINGLE copy of the data, no matter it's string or number or TmTime, you only keep the byte array
	//coz when you edit something in one tab, then you switch to another tab involving the same data, the edit should be reflected/honored
	
	ArrayList<BytesSense> bytessensemap = new ArrayList<BytesSense>();
	
	ArrayListTable animeData = new ArrayListTable(8);
	public static final int ANIMESID = 0;
	public static final int ANIMEMALID = 1;
	public static final int ANIMEMALNAME = 2;
	public static final int ANIMEMALTYPE = 3;
	public static final int ANIMEMALENG = 4;
	public static final int ANIMEMALSYN = 5;
	public static final int ANIMEMALEPI = 6;
	public static final int ANIMEMALRANK = 7;
	
	
	ArrayDeque<Long> candidateSenses = new ArrayDeque<Long>();
}
