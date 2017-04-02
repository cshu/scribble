package griddoor.IIPropSets;

import griddoor.iiSwBasedSpInterfaces.Globals;
import griddoor.util.PrimaryStruct;
import griddoor.util.UtilFuncs;

import java.io.IOException;
import java.net.UnknownHostException;
import java.util.ArrayList;
import java.util.HashMap;

public class SingleInstanceSense extends PrimaryStruct {
	private SingleInstanceSense(long id) {
		this.id = id;
	}
	public long id;
	public ArrayList<String> strs = new ArrayList<>();
	public ArrayList<Long> lons = new ArrayList<>();
	@Override
	public String gets(int ind){
		return strs.get(ind);
	}
	public void sets(int ind, String str){
		while(ind>=strs.size())
			strs.add(null);
		strs.set(ind, str);
	}
	@Override
	public long getl(int ind){
		return lons.get(ind);
	}
	public void setl(int ind, long l){
		while(ind>=lons.size())
			lons.add(null);
		lons.set(ind, l);
	}
	@Override
	public String toString() {
		StringBuilder sb = new StringBuilder();
		for(String s: strs){
			sb.append(s);//undone what  if s contains \n??
			sb.append('`');
		}
		return sb.toString();
	}
	public int heuristicallyDeleteFromFileStorageNDataStInfo() throws UnknownHostException, IOException{
		//allsenses.remove(this.id);//undone you shouldn't remove it unless the deletion succeeds
		Globals.getOuStre().write(Globals.DELETE_FROMFILESTNDATASTINFO);
		UtilFuncs.outputStreamSendInt64(Globals.getOuStre(), this.id);
		return Globals.getInStre().read();
	}
	public static int deleteShortNote(long id) throws UnknownHostException, IOException{
		//allsenses.remove(id);//undone you shouldn't remove it unless the deletion succeeds
		Globals.getOuStre().write(Globals.DELETE_SHORTNOTE);
		UtilFuncs.outputStreamSendInt64(Globals.getOuStre(), id);
		return Globals.getInStre().read();
	}
	public static long upsertShortNoteTextOnly(long id, String text) throws UnknownHostException, IOException{
		Globals.getOuStre().write(Globals.UPSERT_SHORTNOTETEXTONLY);
		UtilFuncs.outputStreamSendInt64(Globals.getOuStre(), id);
		UtilFuncs.outputStreamSendLen24AndStrUtf8(Globals.getOuStre(), text);
		long lbuf = UtilFuncs.inputStreamRecvInt64(Globals.getInStre());
		makeOrGetInstance(lbuf).sets(Globals.SHORTNOTE_text, text);
		return lbuf;
	}
	public static int deleteSAVerb(long id) throws UnknownHostException, IOException{
		//allsenses.remove(id);//undone you shouldn't remove it unless the deletion succeeds
		Globals.getOuStre().write(Globals.DELETE_SAVERB);
		UtilFuncs.outputStreamSendInt64(Globals.getOuStre(), id);
		return Globals.getInStre().read();
	}
	public static int updateSAVerb(long id,String text) throws UnknownHostException, IOException{
		Globals.getOuStre().write(Globals.UPSERT_SAVERB);
		UtilFuncs.outputStreamSendInt64(Globals.getOuStre(), id);
		UtilFuncs.outputStreamSendLen24AndStrUtf8(Globals.getOuStre(), text);
		int valtoret = Globals.getInStre().read();
		if(valtoret==0)
			makeOrGetInstance(id).sets(0, text);
		return valtoret;
	}
	public static long insertSAVerb(String text) throws UnknownHostException, IOException{
		Globals.getOuStre().write(Globals.UPSERT_SAVERB);
		UtilFuncs.outputStreamSendInt64(Globals.getOuStre(), 0L);
		UtilFuncs.outputStreamSendLen24AndStrUtf8(Globals.getOuStre(), text);
		if(Globals.getInStre().read()!=0)
			return 0L;
		long lbuf = UtilFuncs.inputStreamRecvInt64(Globals.getInStre());
		makeOrGetInstance(lbuf).sets(0, text);
		return lbuf;
	}
	public static SingleInstanceSense recvVerb() throws UnknownHostException, IOException{
		long lbuf = UtilFuncs.inputStreamRecvInt64(Globals.getInStre());
		if(lbuf==0)
			return null;
		SingleInstanceSense sis = makeOrGetInstance(lbuf);
		sis.sets(0, UtilFuncs.inputStreamRecvUtf8String(Globals.getInStre()));
		return sis;
	}
	public static SingleInstanceSense recvSAVerb(long lbuf) throws UnknownHostException, IOException{
		SingleInstanceSense sis = makeOrGetInstance(lbuf);
		sis.sets(0,UtilFuncs.inputStreamRecvUtf8String(Globals.getInStre()));
		return sis;
	}
	public static SingleInstanceSense recvShortNoteInstance(long lbuf) throws UnknownHostException, IOException{
		SingleInstanceSense sis = makeOrGetInstance(lbuf);
		sis.sets(Globals.SHORTNOTE_text, UtilFuncs.inputStreamRecvUtf8String(Globals.getInStre()));
		sis.lons.clear();
		do{
			SingleInstanceSense vsens = recvVerb();
			if(vsens == null)
				break;
			sis.lons.add(vsens.id);
		}while(true);
		return sis;
	}
	public static SingleInstanceSense recvShortNoteWithSynopsis() throws UnknownHostException, IOException{
		long lbuf =UtilFuncs.inputStreamRecvInt64(Globals.getInStre());
		if(lbuf==0)
			return null;
		SingleInstanceSense sis = new SingleInstanceSense(lbuf);
		sis.sets(Globals.SHORTNOTE_text, UtilFuncs.inputStreamRecvUtf8String(Globals.getInStre()));
		do{
			SingleInstanceSense vsens = recvVerb();
			if(vsens == null)
				break;
			sis.lons.add(vsens.id);
		}while(true);
		return sis;
	}
	public static SingleInstanceSense selectUrlBookmarkInstance(long lbuf) throws UnknownHostException, IOException{
		Globals.getOuStre().write(Globals.SELECT_URLBOOKMARK);
		UtilFuncs.outputStreamSendInt64(Globals.getOuStre(), lbuf);
		SingleInstanceSense sis = makeOrGetInstance(lbuf);
		sis.sets(0, UtilFuncs.inputStreamRecvUtf8String(Globals.getInStre()));
		sis.sets(1, UtilFuncs.inputStreamRecvUtf8String(Globals.getInStre()));
		return sis;
	}
	public static SingleInstanceSense recvUrlBookmarkInstance() throws UnknownHostException, IOException{
		long lbuf =UtilFuncs.inputStreamRecvInt64(Globals.getInStre());
		if(lbuf==0)
			return null;
		SingleInstanceSense sis = makeOrGetInstance(lbuf);
		sis.sets(0, UtilFuncs.inputStreamRecvUtf8String(Globals.getInStre()));
		sis.sets(1, UtilFuncs.inputStreamRecvUtf8String(Globals.getInStre()));
		return sis;
	}
	public static long upsertUrlBookmark(long id, String url, String desc) throws UnknownHostException, IOException{
		Globals.getOuStre().write(Globals.UPSERT_URLBOOKMARK);
		UtilFuncs.outputStreamSendInt64(Globals.getOuStre(), id);
		UtilFuncs.outputStreamSendLen24AndStrUtf8(Globals.getOuStre(), url);
		UtilFuncs.outputStreamSendLen24AndStrUtf8(Globals.getOuStre(), desc);
		if(Globals.getInStre().read()==0){
			id = UtilFuncs.inputStreamRecvInt64(Globals.getInStre());
			SingleInstanceSense sis = makeOrGetInstance(id);
			sis.sets(0, url);
			sis.sets(1, desc);
			return id;
		} else
			return 0L;
	}
	public static int deleteUrlBookmark(long id) throws UnknownHostException, IOException{
		//allsenses.remove//undone you shouldn't remove it unless the deletion succeeds
		Globals.getOuStre().write(Globals.DELETE_URLBOOKMARK);
		UtilFuncs.outputStreamSendInt64(Globals.getOuStre(), id);
		return Globals.getInStre().read();
	}
	public static SingleInstanceSense recvFileJustAlloc(String onm, long size, long wtime) throws UnknownHostException, IOException{
		long lbuf = UtilFuncs.inputStreamRecvInt64(Globals.getInStre());
		SingleInstanceSense fil = makeOrGetInstance(lbuf);
		fil.sets(0, UtilFuncs.inputStreamRecvUtf8String(Globals.getInStre()));
		fil.sets(1, onm);
		fil.setl(0, size);
		fil.setl(1, wtime);
		return fil;
	}
	public static SingleInstanceSense recvFileInstance() throws UnknownHostException, IOException{
		int byt = Globals.getInStre().read();
		if(byt==0)
			return null;
		long lbuf = UtilFuncs.inputStreamRecvInt64(Globals.getInStre());
		SingleInstanceSense fil = makeOrGetInstance(lbuf);
		fil.sets(Globals.FILE_snm, UtilFuncs.inputStreamRecvUtf8String(Globals.getInStre()));
		fil.setl(Globals.FILE_size, UtilFuncs.inputStreamRecvUnsignedAsLong(Globals.getInStre()));
		fil.setl(Globals.FILE_wtime, UtilFuncs.inputStreamRecv40BitTimeAsLong(Globals.getInStre()));
		fil.sets(Globals.FILE_onm, UtilFuncs.inputStreamRecvUtf8String(Globals.getInStre()));
		return fil;
	}
	public static SingleInstanceSense recvMalAnimeInstance(long lbuf) throws UnknownHostException, IOException{
		SingleInstanceSense malanime = makeOrGetInstance(lbuf);
		malanime.strs.clear();//note this is necessary coz you have variable number of items
		malanime.sets(Globals.ANIMATION_mid, UtilFuncs.inputStreamRecvUtf8String(Globals.getInStre()));
		malanime.sets(Globals.ANIMATION_title, UtilFuncs.inputStreamRecvUtf8String(Globals.getInStre()));
		malanime.sets(Globals.ANIMATION_english, UtilFuncs.inputStreamRecvUtf8String(Globals.getInStre()));
		malanime.sets(Globals.ANIMATION_synonyms, UtilFuncs.inputStreamRecvUtf8String(Globals.getInStre()));
		malanime.sets(Globals.ANIMATION_episodes, UtilFuncs.inputStreamRecvUtf8String(Globals.getInStre()));
		malanime.sets(Globals.ANIMATION_type, UtilFuncs.inputStreamRecvUtf8String(Globals.getInStre()));
		for(int i=Globals.ANIMATION_ess;Globals.getInStre().read()!=0;i++)
			malanime.sets(i, UtilFuncs.inputStreamRecvUtf8String(Globals.getInStre()));
		return malanime;
	}
	public static SingleInstanceSense recvMalAnimeInstance() throws UnknownHostException, IOException{
		long lbuf = UtilFuncs.inputStreamRecvInt64(Globals.getInStre());
		return recvMalAnimeInstance(lbuf);
	}
//	public static SingleInstanceSense recvWordGroup(long lbuf) throws UnknownHostException, IOException{
//		//long lbuf = UtilFuncs.inputStreamRecvInt64(Globals.getInStre());
//		if(lbuf)
//		return null;
//	}
	public static SingleInstanceSense makeOrGetInstance(long id){
		SingleInstanceSense valtoret = allsenses.get(id);
		if(valtoret==null){
			valtoret = new SingleInstanceSense(id);
			allsenses.put(id, valtoret);
		}
		return valtoret;
	}
	public static final HashMap<Long,SingleInstanceSense> allsenses = new HashMap<>();
}
