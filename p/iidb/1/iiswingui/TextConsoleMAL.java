package griddoor.iiswingui;

import griddoor.myanimelist.MALAnimeSearch;

import java.util.ArrayList;

public class TextConsoleMAL extends TextConsoleQueList {

	public TextConsoleMAL() {
		title="MAL";//todo this string is actually useless, (never used)
	}
	
	public ArrayList<QueMALEntry> queMals=new ArrayList<QueMALEntry>();
	public QueMALEntry selectedEntry;
	
	public void setMalEntriesAndRefr(MALAnimeSearch malAnimeSearch){
		//todo maybe highlight already registered animes and already inherited animes?
		queMals.clear();
		malAnimeSearch.fillListWithEntries(queMals);
		
		quesOffset=0;
		ind=1;
		RefrPrintedQues();
	}

	@Override
	public ArrayList<? extends Selectable> getQues() {
		return queMals;
	}

	
}
