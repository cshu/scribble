package griddoor.IIPropSets;

import griddoor.iiSwBasedSpInterfaces.Globals;
import griddoor.util.UtilFuncs;

import java.io.IOException;
import java.net.UnknownHostException;
import java.util.ArrayList;

public class MalAnime {
	public static ArrayList<MalAnime> candidates = new ArrayList<>();
	
	public void upsertToDb() throws UnknownHostException, IOException{
		Globals.getOuStre().write(Globals.UPSERT_MALANIME);
		UtilFuncs.outputStreamSendInt64(Globals.getOuStre(), sensid);
		UtilFuncs.outputStreamSendLen24AndStrUtf8(Globals.getOuStre(), id);
		UtilFuncs.outputStreamSendLen24AndStrUtf8(Globals.getOuStre(), title);
		UtilFuncs.outputStreamSendLen24AndStrUtf8(Globals.getOuStre(), english);
		UtilFuncs.outputStreamSendLen24AndStrUtf8(Globals.getOuStre(), synonyms);
		UtilFuncs.outputStreamSendLen24AndStrUtf8(Globals.getOuStre(), episodes);
		UtilFuncs.outputStreamSendLen24AndStrUtf8(Globals.getOuStre(), type);
		sensid = UtilFuncs.inputStreamRecvInt64(Globals.getInStre());
	}

	//note how to determine whether this MalAnime has been saved in iidb already?
	//case 1. id/title/english, if all 3 are found in a record then that's a already registered anime
	//case 2. id/title/english, none of them are found anywhere, then it doesn't exist
	//case 3. id/title/english, id already used but at least 1 of title/english doesn't match
	//case 4. title/english appeared in either one of title/english field, yet at least 1 of the 3 doesn't match
	//		for case 3 & 4, there will be prompts, user can choose to overwrite(update) it / create new record
	//case 5. when there is no obvious relation between a candidate anime and a registered one, you can still manually overwrite(update), so you can keep using that sense id, just update the attributes
	public void checkCandidateOrRegistry() throws UnknownHostException, IOException{
		if(candidates.contains(this)){
			chkcr = 255;
			return;
		}
		Globals.getOuStre().write(Globals.CHKANIMEREGISTRATION);
		UtilFuncs.outputStreamSendLen24AndStrUtf8(Globals.getOuStre(), id);
		UtilFuncs.outputStreamSendLen24AndStrUtf8(Globals.getOuStre(), title);
		UtilFuncs.outputStreamSendLen24AndStrUtf8(Globals.getOuStre(), english);
		chkcr = Globals.getInStre().read();
		if(chkcr==254){
			sensid = UtilFuncs.inputStreamRecvInt64(Globals.getInStre());
		}
	}
	
	@Override
	public boolean equals(Object obj) {
		if (obj == null) return false;
		if (obj == this) return true;
		if (!(obj instanceof MalAnime))return false;
		MalAnime ma = (MalAnime)obj;
		return this.id.equals(ma.id);
	}
	
	@Override
	public String toString() {
		StringBuilder sb;
		switch(chkcr){
		case 0:
			sb = new StringBuilder("             `");
			break;
		case 255:
			sb = new StringBuilder("Candidate    `");
			break;
		case 254:
			sb = new StringBuilder("Registered   `");
			break;
		case 1:
			sb = new StringBuilder("New          `");
			break;
		default:
			sb = new StringBuilder("With conflict`");
			break;
		}
		sb.append(String.format("%7s", type)).append(String.format("%6s", episodes));
		sb.append(String.format("%22s",status)).append(' ').append(start_date).append(' ').append(end_date);
		sb.append(' ').append(title);
		return sb.toString();
	}
	
	public int chkcr;
	public long sensid;
	
	public String id="";
	public String title="";
	public String english="";
	public String synonyms="";
	public String episodes="";
	public String score="";
	public String type="";
	public String status="";
	public String start_date="";
	public String end_date="";
	public String synopsis="";
	public String image="";

}
