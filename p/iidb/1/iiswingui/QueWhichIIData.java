package griddoor.iiswingui;

import java.util.Arrays;

public class QueWhichIIData extends QueToAnsWithOpt {

	public QueWhichIIData() {
		super("What language/data?");
		opts = Arrays.<CharSequence> asList(
		// "Data Definition Language",
		// "Data Manipulation Language",
		// "Transaction Control Language"
				"Select/update/insert/delete ISNL in table ", "Select/insert(involves create pred tab)/delete(involves drop pred tab) in CLAUSE table", "Select/update/insert/delete in any table (suitable for PREDICATE)", "A simple SQL client for II database", "Connect(Close)/Commit+begin transaction", "Search for Predicates", "Search for files (Shortcut)");
	}

	@Override
	public void FuncSpacePress() {
		if (IISUI.clientSocket == null || IISUI.clientSocket.isClosed()) {
			if (optans == 4) {
				IISUI.tc.AddNewlnAndQue_RefrSelAs2ndLast_Set2ndLastSel_CalcSelectionPos_DrawTextAndSelection(new QueConfirmEstablishConn());
				IISUI.tc.DrawTextAndSelection();
			}
		} else
			switch (optans) {
			case 0:
				IISUI.tc = IISUI.tcSen;
				IISUI.tc.DrawTextAndSelection();
				break;
			case 1:
				IISUI.tc = IISUI.tcCla;
				IISUI.tc.DrawTextAndSelection();
				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				IISUI.tc.AddNewlnAndQue_RefrSelAs2ndLast_Set2ndLastSel_CalcSelectionPos_DrawTextAndSelection(new QueCloseConnOrTCL());
				IISUI.tc.DrawTextAndSelection();
				break;
			case 5:
				IISUI.tc = IISUI.tcPre;
				IISUI.tc.DrawTextAndSelection();
				break;
			case 6:// undone
				break;
			}
	}
}
