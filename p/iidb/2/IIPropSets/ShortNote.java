package griddoor.IIPropSets;

import java.util.ArrayList;

public class ShortNote {

	//todo: not urgent coz your server doesn't accept concurrent clients for now
	//you need to define Precondition Unmodified Props and Modifications
	//the modifications are only carried out if the precondition unmodified props are confirmed unmodified (including nonexistent, that is to say new props also count as modifications)
	//this is for concurrent modifications.
	
	//todo: not urgent coz it's complicated to implement, and user can be careful to avoid the issue
	//Almost the same as above, you have 2 tabs, each has modifications filled in forms
	//after you send modifications of tab 1, DB is updated
	//according to solution above, when you send modifications of tab 2, it should fail
	//but shouldn't the use get notified before sending the request? coz both tabs are at the same client
	//when one tab commits changes to DB, the other tab should get the information about changes-during-update?
	
	//
	//todo:
	//and there is another case which seems similar but is actually totally different:
	
//e.g.:
//	for a single sense id
//
//	used twice for 2 features in client side
//
//	in one feature, client side reassigned a sense to the id
//
//	but the other feature doesn't know about the reassignment, it tries to save some props related to that sense id
	//
	//solution is to use EXTREME Precondition unmodified props and modifications + leftout checking
	//that is, not only to guarantee it's unmodified, but also check that there is no LEFTOUT props
	//when there is LEFTOUT props, you should prompt them to let user decide whether continue modifications or use new sense id
	
	

	public long bytesSenseId;//0 means new note
	public String javaStr;
	public ArrayList<Long> singleArgVerbs;

}
