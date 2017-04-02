package griddoor.iiswingui;

import griddoor.util.UtilFuncs;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.util.Arrays;

import javax.swing.JOptionPane;

public class QueMALEntry extends QueToAnsWithOpt {

	public QueMALEntry(long id, String title, String english, String synonyms, long episodes, String score, String type) {
		super(title);
		this.id = id;
		this.title = title;
		this.english = english;
		this.synonyms = synonyms;
		this.episodes = episodes;
		this.score = score;
		this.type = type;
		opts = Arrays.<CharSequence> asList("View digital files", "Mark fully inherited");

		byte[] oldTitle;
		String oldTitStr;
		byte[] oldType;
		String oldTypStr;
		byte[] oldEnglish;
		String oldEngStr;
		byte[] oldSynonyms;
		String oldSynStr;
		byte[] oldEpisodes;
		long oldEpiLong;
		boolean updateE = false;
		boolean updateQ = false;
		boolean updateP = false;
		boolean updateAbandoned = false;
		long animeIdInLong = 0;
		blk_of_animeupdate: try {
			switch (Utils.RequestAnimeFromMalAndRead1stByt(id)) {
			case 0:
				allText=allText+" (Registered)";
				UtilFuncs.InputStreamReadBytesUntil(8, IISUI.inpuStreSo, animeId = new byte[8], 0);
				animeIdInLong = UtilFuncs.GetLongFromLE8Bytes(animeId, 0);
				oldTitle = new byte[IISUI.inpuStreSo.read() + IISUI.inpuStreSo.read() * 256];
				UtilFuncs.InputStreamReadBytesUntil(oldTitle.length, IISUI.inpuStreSo, oldTitle, 0);
				oldTitStr = new String(oldTitle, StandardCharsets.UTF_8);
				if (!oldTitStr.equals(title)) {//ask for confirmation in case it becomes a totally different anime
					if (JOptionPane.showConfirmDialog(IISUI.theJfr, "Update with the new title?", oldTitStr + "\n  ---------------\n" + title, JOptionPane.OK_CANCEL_OPTION) == JOptionPane.OK_OPTION)
						updateE = true;
					else
						updateAbandoned = true;
				}
				oldType = new byte[IISUI.inpuStreSo.read() + IISUI.inpuStreSo.read() * 256];
				UtilFuncs.InputStreamReadBytesUntil(oldType.length, IISUI.inpuStreSo, oldType, 0);
				oldTypStr = new String(oldType, StandardCharsets.UTF_8);
				if (!oldTypStr.equals(type)) {
					if (JOptionPane.showConfirmDialog(IISUI.theJfr, "Update with the new type?", oldTypStr + "\n  ---------------\n" + type, JOptionPane.OK_CANCEL_OPTION) == JOptionPane.OK_OPTION)
						updateE = true;
					else
						updateAbandoned = true;
				}
				if (IISUI.inpuStreSo.read() != 1)//if there are more rows to return
					IISUI.sysOut.atAlias.append("\nUnexpected input read from server!");
				break;
			case 1:
				break blk_of_animeupdate;
			default:
				//todo: Handle unexpected input?
				break;
			}
			if(updateAbandoned)
				break blk_of_animeupdate;
			switch (Utils.RequestQFromAIdAndRead1stByt(animeIdInLong)) {
			case 0:
				allText=allText+" (Detailed)";
				oldEnglish = new byte[IISUI.inpuStreSo.read() + IISUI.inpuStreSo.read() * 256];
				UtilFuncs.InputStreamReadBytesUntil(oldEnglish.length, IISUI.inpuStreSo, oldEnglish, 0);
				oldEngStr = new String(oldEnglish, StandardCharsets.UTF_8);
				if (!oldEngStr.equals(english))
					updateQ = true;
				oldSynonyms = new byte[IISUI.inpuStreSo.read() + IISUI.inpuStreSo.read() * 256];
				UtilFuncs.InputStreamReadBytesUntil(oldSynonyms.length, IISUI.inpuStreSo, oldSynonyms, 0);
				oldSynStr = new String(oldSynonyms, StandardCharsets.UTF_8);
				if (!oldSynStr.equals(synonyms))
					updateQ = true;
				oldEpisodes = new byte[IISUI.inpuStreSo.read() + IISUI.inpuStreSo.read() * 256];
				UtilFuncs.InputStreamReadBytesUntil(oldEpisodes.length, IISUI.inpuStreSo, oldEpisodes, 0);
				oldEpiLong = UtilFuncs.GetLongFromLEBytes(oldEpisodes, 0);
				if (oldEpiLong != episodes) {
					if (JOptionPane.showConfirmDialog(IISUI.theJfr, "Update with the new episodes?", oldEpiLong + "\n  ---------------\n" + episodes, JOptionPane.OK_CANCEL_OPTION) == JOptionPane.OK_OPTION)
						updateQ = true;
					else
						updateAbandoned = true;
				}
				if (IISUI.inpuStreSo.read() != 1)//if there are more rows to return
					IISUI.sysOut.atAlias.append("\nUnexpected input read from server!");
				break;
			case 1:
				switch (Utils.RequestPFromAIdAndRead1stByt(animeIdInLong)){
				case 0:
					allText=allText+" (FULLY inherited)";
					oldEnglish = new byte[IISUI.inpuStreSo.read() + IISUI.inpuStreSo.read() * 256];
					UtilFuncs.InputStreamReadBytesUntil(oldEnglish.length, IISUI.inpuStreSo, oldEnglish, 0);
					oldEngStr = new String(oldEnglish, StandardCharsets.UTF_8);
					if (!oldEngStr.equals(english))
						updateP = true;
					oldSynonyms = new byte[IISUI.inpuStreSo.read() + IISUI.inpuStreSo.read() * 256];
					UtilFuncs.InputStreamReadBytesUntil(oldSynonyms.length, IISUI.inpuStreSo, oldSynonyms, 0);
					oldSynStr = new String(oldSynonyms, StandardCharsets.UTF_8);
					if (!oldEngStr.equals(english))
						updateP = true;
					oldEpisodes = new byte[IISUI.inpuStreSo.read() + IISUI.inpuStreSo.read() * 256];
					UtilFuncs.InputStreamReadBytesUntil(oldEpisodes.length, IISUI.inpuStreSo, oldEpisodes, 0);
					oldEpiLong = UtilFuncs.GetLongFromLEBytes(oldEpisodes, 0);
					if (oldEpiLong != episodes) {
						if (JOptionPane.showConfirmDialog(IISUI.theJfr, "Update with the new episodes?", oldEpiLong + "\n  ---------------\n" + episodes, JOptionPane.OK_CANCEL_OPTION) == JOptionPane.OK_OPTION)
							updateP = true;
						else
							updateAbandoned = true;
					}
					if (IISUI.inpuStreSo.read() != 1)//if there are more rows to return
						IISUI.sysOut.atAlias.append("\nUnexpected input read from server!");
					break;
				case 1://in case no info is found in P or Q, add it to Q
					Utils.InsertIntoQ(animeId, english, synonyms, episodes);
					break;
				default:
					//todo: Handle unexpected input?
					break;
				}
				break;
			default:
				//todo: Handle unexpected input?
				break;
			}
			if(updateAbandoned)
				break blk_of_animeupdate;

			if (updateE) {
				IISUI.outpStreSo.write(4);
				UtilFuncs.OutputStreamSendLen16AndStrUtf8(IISUI.outpStreSo, "UPDATE e SET b=?,c=? WHERE a=" + animeIdInLong);
				IISUI.outpStreSo.write(0);
				UtilFuncs.OutputStreamSendLen16AndStrUtf8(IISUI.outpStreSo, title);
				IISUI.outpStreSo.write(0);
				UtilFuncs.OutputStreamSendLen16AndStrUtf8(IISUI.outpStreSo, type);
				IISUI.outpStreSo.write(1);
				if (IISUI.inpuStreSo.read() != 1)
					IISUI.sysOut.atAlias.append("\nUnexpected input read from server!");
			}
			if(updateQ||updateP){
				IISUI.outpStreSo.write(4);
				UtilFuncs.OutputStreamSendLen16AndStrUtf8(IISUI.outpStreSo, "UPDATE "+(updateQ?"q":"p")+" SET b=?,c=?,d=? WHERE a=" + animeIdInLong);
				IISUI.outpStreSo.write(0);
				UtilFuncs.OutputStreamSendLen16AndStrUtf8(IISUI.outpStreSo, english);
				IISUI.outpStreSo.write(0);
				UtilFuncs.OutputStreamSendLen16AndStrUtf8(IISUI.outpStreSo, synonyms);
				IISUI.outpStreSo.write(0);
				UtilFuncs.OutputStreamSendLen16AndLongSignifBytes(IISUI.outpStreSo, episodes);
				IISUI.outpStreSo.write(1);
				if (IISUI.inpuStreSo.read() != 1)
					IISUI.sysOut.atAlias.append("\nUnexpected input read from server!");
			}
		} catch (IOException e) {
			e.printStackTrace();
			IISUI.sysOut.atAlias.append("\n" + e.toString());
		}
	}

	public long id;
	public String title;
	public String english;
	public String synonyms;
	public long episodes;
	public String score;
	public String type;
	public String status;
	public String start_date;
	public String end_date;
	public String synopsis;
	public String image;
	public String xml;

	public byte[] animeId;

	@Override
	public void FuncFocusGained() {
		IISUI.tcMal.selectedEntry = this;
		if (QueFile.inheritsProduct == 2) {
			QueFile.updateJFrameTitle(title);
		}
	}

	@Override
	public void RefrSelAs2ndLast() {
		super.RefrSelAs2ndLast();
		s2Alias.append('\n');
		s2Alias.append(xml);
	}

	@Override
	public void FuncSpacePress() {
		switch (optans) {
		case 0:
			//undone switch to a new Prop Console
			break;
		case 1:
			if (animeId == null)//this anime's not even inherited
				return;
			//undone
			break;
		}
	}

}
