package griddoor.IIPropSets;

import griddoor.iiSwBasedSpInterfaces.Globals;

import java.io.IOException;
import java.io.OutputStream;
import java.net.UnknownHostException;
import java.util.ArrayList;

public class Verb {
	public static ArrayList<Verb> candidates = new ArrayList<>();
	public static Verb makeNewVerbWithOneArg(){
		Verb v = new Verb();
		v.args = new ArrayList<VerbArg>(1);
		v.args.add(new VerbArg());
		return v;
	}
	public static Verb recvVerbFromSvr() throws UnknownHostException, IOException{
		int byt = Globals.getInStre().read();
		if(byt!=0)
			return null;
		Verb v = new Verb();
		v.id = griddoor.util.UtilFuncs.inputStreamRecvInt64(Globals.getInStre());
		//debug no verb desc
		v.desc = griddoor.util.UtilFuncs.inputStreamRecvUtf8String(Globals.getInStre());
		v.args = new ArrayList<VerbArg>(1);
		v.args.add(new VerbArg());
		//debug single arg
		v.args.get(0).id = 1;//griddoor.util.UtilFuncs.InputStreamRecvInt64(Globals.getInStre());
		//v.args.get(0).desc = griddoor.util.UtilFuncs.inputStreamRecvUtf8String(Globals.getInStre());
		v.entailmentSets = new ArrayList<>();
//		while(Globals.getInStre().read()==0){
//			EntailmentSet newes = new EntailmentSet();
//			newes.formulatype = (byte)Globals.getInStre().read();
//			newes.verbs = new ArrayList<>();
//			for(int numOfV = griddoor.util.UtilFuncs.inputStreamRecvInt32(Globals.getInStre()); numOfV!=0; numOfV--)
//				newes.verbs.add(griddoor.util.UtilFuncs.InputStreamRecvInt64(Globals.getInStre()));
//			v.entailmentSets.add(newes);
//		}
		return v;
	}
	
	
	
	//each query sent to server can only retrieve data under the amount limit
//the limit is set at server side
	
	
	public boolean newVerbElseEditVerb;
	
	
	
	
	
	public long id;
	public String desc;
	public ArrayList<VerbArg> args;
	public ArrayList<EntailmentSet> entailmentSets;
	@Override
	public String toString() {
		StringBuilder sb = new StringBuilder().append(desc);
		for(VerbArg arg : args)
			sb.append('`').append(arg.desc);
		return sb.toString();
	}
	
	public int submitNewVerb() throws IOException{
		Globals.getOuStre().write(Globals.NEW_V);
		//debug not sending verb desc, sending only 1st arg desc
		griddoor.util.UtilFuncs.outputStreamSendLen24AndStrUtf8(Globals.getOuStre(), args.get(0).desc);
//		griddoor.util.UtilFuncs.OutputSreamSendInt32(Globals.getOuStre(), entailmentSets.size());
//		for(EntailmentSet es : entailmentSets){
//			Globals.getOuStre().write(es.formulatype);
//			for(long l: es.verbs){
//				Globals.getOuStre().write(es.formulatype);
//				griddoor.util.UtilFuncs.OutputSreamSendInt64(Globals.getOuStre(), l);
//			}
//		}
		return Globals.getInStre().read();
	}
}

