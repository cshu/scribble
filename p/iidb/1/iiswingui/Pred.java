package griddoor.iiswingui;

import griddoor.iiswingui.IISUI.BI;
import griddoor.util.UtilFuncs;

import java.util.ArrayList;
import java.util.Arrays;

public class Pred extends QueToAns {

	private Pred(CharSequence allText) {
		super(allText);
	}

	public ArrayList<ClauRecord> clauses;

	public static Pred Make(ArrayList<ClauRecord> clauses) {
		StringBuilder quetext = new StringBuilder(clauses.size() + " clauses~");
		for (ClauRecord cr : clauses) {
			if (cr.sense != null)
				for (BI bi : IISUI.synonym)
					if (Arrays.equals(bi.b, cr.sense)) {
						quetext.append(bi.a);
						quetext.append(':');
						if (cr.insid != null)
							quetext.append(cr.insid);
						else if(cr.digorbyt!=null)
							quetext.append("Dig/Byt");
						quetext.append('~');
						break;
					}
		}
		Pred p = new Pred(quetext);
		p.clauses = clauses;
		StringBuilder fulltext = new StringBuilder("++++++++++++++++++++++++++++\n ");
		for(ClauRecord cr : clauses){
			switch(cr.typenum){
			case 2:
				fulltext.append(IISUI.listOfClauseTypes.get(cr.typenum-1));
				fulltext.append(" NSeg");
				fulltext.append(UtilFuncs.GetLongFromLE8Bytes(cr.nseg1,0));
				fulltext.append(" VSeg");
				fulltext.append(UtilFuncs.GetLongFromLE8Bytes(cr.vseg1,0));
				fulltext.append(" NSeg");
				fulltext.append(UtilFuncs.GetLongFromLE8Bytes(cr.nseg2,0));
				fulltext.append("\n ");
				break;
			case 3:
				fulltext.append(IISUI.listOfClauseTypes.get(cr.typenum-1));
				fulltext.append(" NSeg");
				fulltext.append(UtilFuncs.GetLongFromLE8Bytes(cr.nseg1,0));
				fulltext.append(' ');
				fulltext.append(Utils.GetSynAFromB(cr.sense));
				fulltext.append("\n ");
				break;
			case 4:
				//undone connounins
				break;
			case 5:
				fulltext.append(IISUI.listOfClauseTypes.get(cr.typenum-1));
				fulltext.append(" NSeg");
				fulltext.append(UtilFuncs.GetLongFromLE8Bytes(cr.nseg1,0));
				fulltext.append(' ');
				fulltext.append(Utils.GetSynAFromB(cr.sense));
				fulltext.append("\n ");
				break;
			case 6:
				//undone bytnounins
				break;
			case 7:
				fulltext.append(IISUI.listOfClauseTypes.get(cr.typenum-1));
				fulltext.append(" NSeg");
				fulltext.append(UtilFuncs.GetLongFromLE8Bytes(cr.nseg1,0));
				fulltext.append(' ');
				fulltext.append(Utils.GetSynAFromB(cr.sense));
				fulltext.append("\n ");
				break;
			case 8:
				//undone dignounins
				break;
			case 9:
			case 11:
				fulltext.append(IISUI.listOfClauseTypes.get(cr.typenum-1));
				fulltext.append(" NSeg");
				fulltext.append(UtilFuncs.GetLongFromLE8Bytes(cr.nseg1,0));
				fulltext.append("\n ");
				break;
			case 10:
				//undone rawbyteins
				break;
			case 12:
				//undone rawdigiins
				break;
			case 13:
			case 15:
				fulltext.append(IISUI.listOfClauseTypes.get(cr.typenum-1));
				fulltext.append(" VSeg");
				fulltext.append(UtilFuncs.GetLongFromLE8Bytes(cr.vseg1,0));
				fulltext.append(' ');
				fulltext.append(Utils.GetSynAFromB(cr.sense));
				fulltext.append("\n ");
				break;
			case 14:
			case 16:
				fulltext.append(IISUI.listOfClauseTypes.get(cr.typenum-1));
				fulltext.append(" NSeg");
				fulltext.append(UtilFuncs.GetLongFromLE8Bytes(cr.nseg1,0));
				fulltext.append(' ');
				fulltext.append(Utils.GetSynAFromB(cr.sense));
				fulltext.append("\n ");
				break;
			case 17:
				fulltext.append(IISUI.listOfClauseTypes.get(cr.typenum-1));
				fulltext.append(" NSeg");
				fulltext.append(UtilFuncs.GetLongFromLE8Bytes(cr.nseg1,0));
				fulltext.append(" VSeg");
				fulltext.append(UtilFuncs.GetLongFromLE8Bytes(cr.vseg1,0));
				fulltext.append(" VSeg");
				fulltext.append(UtilFuncs.GetLongFromLE8Bytes(cr.vseg2,0));
				fulltext.append("\n ");
				break;
			case 18:
				fulltext.append(IISUI.listOfClauseTypes.get(cr.typenum-1));
				fulltext.append(" VSeg");
				fulltext.append(UtilFuncs.GetLongFromLE8Bytes(cr.vseg1,0));
				fulltext.append(" VSeg");
				fulltext.append(UtilFuncs.GetLongFromLE8Bytes(cr.vseg2,0));
				fulltext.append(" NSeg");
				fulltext.append(UtilFuncs.GetLongFromLE8Bytes(cr.nseg1,0));
				fulltext.append("\n ");
				break;
			case 19:
				fulltext.append(IISUI.listOfClauseTypes.get(cr.typenum-1));
				fulltext.append(" NSeg");
				fulltext.append(UtilFuncs.GetLongFromLE8Bytes(cr.nseg1,0));
				fulltext.append(" VSeg");
				fulltext.append(UtilFuncs.GetLongFromLE8Bytes(cr.vseg1,0));
				fulltext.append(" NSeg");
				fulltext.append(UtilFuncs.GetLongFromLE8Bytes(cr.nseg2,0));
				fulltext.append("\n ");
				break;
			case 20:
				fulltext.append(IISUI.listOfClauseTypes.get(cr.typenum-1));
				fulltext.append(" NSeg");
				fulltext.append(UtilFuncs.GetLongFromLE8Bytes(cr.nseg1,0));
				fulltext.append(" VSeg");
				fulltext.append(UtilFuncs.GetLongFromLE8Bytes(cr.vseg1,0));
				fulltext.append(" NSeg");
				fulltext.append(UtilFuncs.GetLongFromLE8Bytes(cr.nseg2,0));
				fulltext.append("\n ");
				break;
			}
		}
		p.selAs2ndLast.allText = fulltext;
		return  p;
	}

}
