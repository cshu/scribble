package griddoor.iiswingui;

import griddoor.iiswingui.IISUI.BI;
import griddoor.iiswingui.IISUI.IB;
import griddoor.iiswingui.IISUI.II;
import griddoor.iiswingui.IISUI.III;
import griddoor.iiswingui.IISUI.IIII;
import griddoor.util.UtilFuncs;

import java.io.File;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.Arrays;

public class Utils {

	public static File GetFileFromReadablePath(String path) {
		return new File(path.startsWith("\"") && path.endsWith("\"") ? path.substring(1, path.length() - 1) : path);
	}

	public static void ReplSynWithUpgradedSens(byte[] code, Selectable sel) {
		for (int i = 0; i < IISUI.tcSen.sels.size(); i++) {
			if (IISUI.tcSen.sels.get(i) instanceof SensSyn && Arrays.equals(((SensSyn) IISUI.tcSen.sels.get(i)).syn.b, code)) {
				IISUI.tcSen.sels.set(i, sel);
			}
		}
		if (IISUI.tcSen.croppedSel1 instanceof SensSyn && Arrays.equals(((SensSyn) IISUI.tcSen.croppedSel1).syn.b, code)) {
			IISUI.tcSen.croppedSel1 = sel;
		}

	}

	public static void RemoveAllOtherQueInThisLine(Selectable sel) {
		int nextNewlnInd = nextNewlnInd();
		IISUI.tc.ind = prevNewlnInd() + 1;
		IISUI.tc.sels.set(IISUI.tc.ind, sel);
		IISUI.tc.sels.subList(IISUI.tc.ind + 1, nextNewlnInd).clear();
	}

	public static Selectable GetSensDbacniOrSynFromSyn(BI syn, CharSequence directIdTable, byte[] clue) {

		for (IB ib : IISUI.dignoun)
			if (Arrays.equals(clue, ib.a)) {
				return new SensDigNoun(syn, directIdTable == null ? "dignoun" : directIdTable);
			}
		for (IB ib : IISUI.bytnoun)
			if (Arrays.equals(clue, ib.a)) {
				return new SensBytNoun(syn, directIdTable == null ? "bytnoun" : directIdTable);
			}
		for (IB ib : IISUI.absroot)
			if (Arrays.equals(clue, ib.a)) {
				return new SensAbsNoun(syn, directIdTable == null ? "absroot" : directIdTable);
			}
		for (IB ib : IISUI.conroot)
			if (Arrays.equals(clue, ib.a)) {
				return new SensConNoun(syn, directIdTable == null ? "conroot" : directIdTable);
			}
		for (IB ib : IISUI.noninvertib)
			if (Arrays.equals(clue, ib.a)) {
				return new SensNoninvV(syn, directIdTable == null ? "noninvertib" : directIdTable);
			}
		for (IB ib : IISUI.invertiblev)
			if (Arrays.equals(clue, ib.a)) {
				return new SensInvertV(syn, directIdTable == null ? "invertiblev" : directIdTable);
			}
		for (II ii : IISUI.subcate1)
			if (Arrays.equals(clue, ii.b))
				return GetSensDbacniOrSynFromSyn(syn, directIdTable == null ? "subcate1" : directIdTable, ii.a);
		for (III iii : IISUI.subcate2)
			if (Arrays.equals(clue, iii.b) || Arrays.equals(clue, iii.c))
				return GetSensDbacniOrSynFromSyn(syn, directIdTable == null ? "subcate2" : directIdTable, iii.a);
		for (III iii : IISUI.inter2)
			if (Arrays.equals(clue, iii.a))
				return GetSensDbacniOrSynFromSyn(syn, directIdTable == null ? "inter2" : directIdTable, iii.b);
		for (III iii : IISUI.union2)
			if (Arrays.equals(clue, iii.b) || Arrays.equals(clue, iii.c))
				return GetSensDbacniOrSynFromSyn(syn, directIdTable == null ? "union2" : directIdTable, iii.a);
		for (IIII iiii : IISUI.inter3)
			if (Arrays.equals(clue, iiii.a))
				return GetSensDbacniOrSynFromSyn(syn, directIdTable == null ? "inter3" : directIdTable, iiii.b);
		for (IIII iiii : IISUI.union3)
			if (Arrays.equals(clue, iiii.b) || Arrays.equals(clue, iiii.c) || Arrays.equals(clue, iiii.d))
				return GetSensDbacniOrSynFromSyn(syn, directIdTable == null ? "union3" : directIdTable, iiii.a);
		for (III iii : IISUI.relacomp2)
			if (Arrays.equals(clue, iii.b) || Arrays.equals(clue, iii.c))
				return GetSensDbacniOrSynFromSyn(syn, directIdTable == null ? "relacomp2" : directIdTable, iii.a);
		for (IIII iiii : IISUI.relacomp3)
			if (Arrays.equals(clue, iiii.b) || Arrays.equals(clue, iiii.c) || Arrays.equals(clue, iiii.d))
				return GetSensDbacniOrSynFromSyn(syn, directIdTable == null ? "relacomp3" : directIdTable, iiii.a);
		return new SensSyn(syn);
	}

	public static Selectable MacrRefrSelAs2ndLast(Selectable tmp) {
		tmp.RefrSelAs2ndLast();
		return tmp;
	}

	public static int prevNewlnInd() {
		for (int i = IISUI.tc.ind - 1;; i--) {
			if (IISUI.tc.sels.get(i) == IISUI.newlineSel)
				return i;
		}
	}

	public static int nextNewlnInd() {
		for (int i = IISUI.tc.ind + 1;; i++) {
			if (IISUI.tc.sels.get(i) == IISUI.newlineSel)
				return i;
		}
	}

	interface I1inBout<I> {
		boolean execFunc(I in);
	}

	public static boolean TraceSenseIdRetB(byte[] b, I1inBout<byte[]> fp) {
		for (II ii : IISUI.subcate1)
			if (Arrays.equals(b, ii.b))
				return fp.execFunc(ii.a);
		for (III iii : IISUI.subcate2)
			if (Arrays.equals(b, iii.b) || Arrays.equals(b, iii.c))
				return fp.execFunc(iii.a);
		for (III iii : IISUI.inter2)
			if (Arrays.equals(b, iii.a))
				return fp.execFunc(iii.b);
		for (III iii : IISUI.union2)
			if (Arrays.equals(b, iii.b) || Arrays.equals(b, iii.c))
				return fp.execFunc(iii.a);
		for (IIII iiii : IISUI.inter3)
			if (Arrays.equals(b, iiii.a))
				return fp.execFunc(iiii.b);
		for (IIII iiii : IISUI.union3)
			if (Arrays.equals(b, iiii.b) || Arrays.equals(b, iiii.c) || Arrays.equals(b, iiii.d))
				return fp.execFunc(iiii.a);
		for (III iii : IISUI.relacomp2)
			if (Arrays.equals(b, iii.b) || Arrays.equals(b, iii.c))
				return fp.execFunc(iii.a);
		for (IIII iiii : IISUI.relacomp3)
			if (Arrays.equals(b, iiii.b) || Arrays.equals(b, iiii.c) || Arrays.equals(b, iiii.d))
				return fp.execFunc(iiii.a);
		return false;
	}

	public static boolean IsConNoun(final byte[] b) {
		for (IB ib : IISUI.conroot)
			if (Arrays.equals(b, ib.a)) {
				return true;
			}
		return TraceSenseIdRetB(b, new I1inBout<byte[]>() {
			@Override
			public boolean execFunc(byte[] in) {
				return IsConNoun(in);
			}
		});
	}

	public static boolean IsDigNoun(final byte[] b) {
		for (IB ib : IISUI.dignoun)
			if (Arrays.equals(b, ib.a)) {
				return true;
			}
		return TraceSenseIdRetB(b, new I1inBout<byte[]>() {
			@Override
			public boolean execFunc(byte[] in) {
				return IsDigNoun(in);
			}
		});
	}

	public static boolean IsBytNoun(byte[] b) {
		for (IB ib : IISUI.bytnoun)
			if (Arrays.equals(b, ib.a)) {
				return true;
			}
		return TraceSenseIdRetB(b, new I1inBout<byte[]>() {
			@Override
			public boolean execFunc(byte[] in) {
				return IsBytNoun(in);
			}
		});
	}

	public static boolean IsVerb(byte[] b) {
		for (IB ib : IISUI.invertiblev)
			if (Arrays.equals(b, ib.a)) {
				return true;
			}
		for (IB ib : IISUI.noninvertib)
			if (Arrays.equals(b, ib.a)) {
				return true;
			}
		return TraceSenseIdRetB(b, new I1inBout<byte[]>() {
			@Override
			public boolean execFunc(byte[] in) {
				return IsVerb(in);
			}
		});
	}

	public static boolean IsCsUsedAsSegInAnyClause(CharSequence allText) {
		boolean newlnFound = false;
		for (Selectable sel : IISUI.tcCla.sels) {
			if (sel == IISUI.newlineSel) {
				newlnFound = true;
				continue;
			}
			if (newlnFound) {
				newlnFound = false;
				continue;
			}
			if (sel.allText == allText) {
				return true;
			}
		}
		return false;
	}

	public static String GetSynAFromB(byte[] bytes) {
		for (BI bi : IISUI.synonym)
			if (Arrays.equals(bi.b, bytes))
				return bi.a;
		return null;
	}

	// public static BI GetSynFromId(byte[] id){
	// for(BI bi : IISUI.)
	// }

	// public static boolean IsParag1stCharNewln(int i) {
	// return IISUI.currTC.parags.get(i).allText.charAt(0) == '\n';
	// }

	public static int RequestAnimeIdFromMalAndRead1stByt(long id) throws IOException {
		IISUI.outpStreSo.write(4);
		UtilFuncs.OutputStreamSendLen16AndStrUtf8(IISUI.outpStreSo, "SELECT a FROM e WHERE d=?");
		IISUI.outpStreSo.write(0);
		UtilFuncs.OutputStreamSendLen16AndLongSignifBytes(IISUI.outpStreSo, id);
		IISUI.outpStreSo.write(1);
		return IISUI.inpuStreSo.read();
	}

	public static int RequestAnimeFromMalAndRead1stByt(long id) throws IOException {
		IISUI.outpStreSo.write(4);
		UtilFuncs.OutputStreamSendLen16AndStrUtf8(IISUI.outpStreSo, "SELECT a,b,c FROM e WHERE d=?");
		IISUI.outpStreSo.write(0);
		UtilFuncs.OutputStreamSendLen16AndLongSignifBytes(IISUI.outpStreSo, id);
		IISUI.outpStreSo.write(1);
		return IISUI.inpuStreSo.read();
	}

	public static int RequestPFromAIdAndRead1stByt(long aid) throws IOException {
		IISUI.outpStreSo.write(4);
		UtilFuncs.OutputStreamSendLen16AndStrUtf8(IISUI.outpStreSo, "SELECT b,c,d FROM p WHERE a="+aid);
		IISUI.outpStreSo.write(1);
		return IISUI.inpuStreSo.read();
	}

	public static int RequestQFromAIdAndRead1stByt(long aid) throws IOException {
		IISUI.outpStreSo.write(4);
		UtilFuncs.OutputStreamSendLen16AndStrUtf8(IISUI.outpStreSo, "SELECT b,c,d FROM q WHERE a="+aid);
		IISUI.outpStreSo.write(1);
		return IISUI.inpuStreSo.read();
	}

	public static byte[] GetOrInsertAnimeIdFromMal() {
		if (IISUI.tcMal.selectedEntry.animeId != null)
			return IISUI.tcMal.selectedEntry.animeId;
		byte[] animeId = new byte[8];
		try {
			switch (RequestAnimeIdFromMalAndRead1stByt(IISUI.tcMal.selectedEntry.id)) {
			case 0:
				UtilFuncs.InputStreamReadBytesUntil(8, IISUI.inpuStreSo, animeId, 0);
				if (IISUI.inpuStreSo.read() != 1)//if there are more rows to return
					IISUI.sysOut.atAlias.append("\nUnexpected input read from server!");
				break;
			case 1:
				IISUI.outpStreSo.write(5);
				UtilFuncs.OutputStreamSendLen16AndStrUtf8(IISUI.outpStreSo, "e");

				IISUI.outpStreSo.write(1);//new instance of ANIME
				UtilFuncs.InputStreamReadBytesUntil(8, IISUI.inpuStreSo, animeId, 0);
				UtilFuncs.OutputStreamSendLen16AndStrUtf8(IISUI.outpStreSo, IISUI.tcMal.selectedEntry.title);
				UtilFuncs.OutputStreamSendLen16AndStrUtf8(IISUI.outpStreSo, IISUI.tcMal.selectedEntry.type);
				UtilFuncs.OutputStreamSendLen16AndLongSignifBytes(IISUI.outpStreSo, IISUI.tcMal.selectedEntry.id);
				IISUI.outpStreSo.write(0);//write rank
				IISUI.outpStreSo.write(0);
				IISUI.outpStreSo.write(0);//write score
				IISUI.outpStreSo.write(0);
				//write int64
				IISUI.outpStreSo.write(0);
				//write 0 for product series
				IISUI.outpStreSo.write(0);
				IISUI.outpStreSo.write(0);
				IISUI.outpStreSo.write(0);
				IISUI.outpStreSo.write(0);
				IISUI.outpStreSo.write(0);
				IISUI.outpStreSo.write(0);
				IISUI.outpStreSo.write(0);
				IISUI.outpStreSo.write(0);

				InsertIntoQ(animeId, IISUI.tcMal.selectedEntry.english, IISUI.tcMal.selectedEntry.synonyms, IISUI.tcMal.selectedEntry.episodes);
				break;
			default:
				//todo: Handle unexpected input?
				break;
			}
		} catch (IOException e) {
			e.printStackTrace();
			IISUI.sysOut.atAlias.append("\n" + e.toString());
		}
		return animeId;
	}

	public static void InsertIntoQ(byte[] animeId, String english, String synonyms, long episodes) throws IOException {
		IISUI.outpStreSo.write(5);
		UtilFuncs.OutputStreamSendLen16AndStrUtf8(IISUI.outpStreSo, "q");
		IISUI.outpStreSo.write(0);
		IISUI.outpStreSo.write(animeId);
		UtilFuncs.OutputStreamSendLen16AndStrUtf8(IISUI.outpStreSo, english);
		UtilFuncs.OutputStreamSendLen16AndStrUtf8(IISUI.outpStreSo, synonyms);
		UtilFuncs.OutputStreamSendLen16AndLongSignifBytes(IISUI.outpStreSo, episodes);
	}
}
