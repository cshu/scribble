package griddoor.IIPropSets;

import java.util.ArrayList;

public class EntailmentSet {
	public static final int ROUNDROBINMUTUALEXCLUSIONS = 1;
	public static final int MASSVERBSEntailTHISONE = 2;
	public static final int MASSVERBSENTAILEDByTHISONE = 3;
	public static final int SOPHISTICATEDSINGLEENTAILMENT = 4;
	
	public byte formulatype;
	
	
	public ArrayList<Long> verbs;//debug for now it only support dull single-arg verbs
}
