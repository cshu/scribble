package griddoor.iiswingui;

import java.io.IOException;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.Arrays;

public class QueConfirmEstablishConn extends QueToAnsWithOpt {

	public QueConfirmEstablishConn() {
		super("Try connecting to server?");
		opts = Arrays.<CharSequence>asList( "Confirm" );
	}

	@Override
	public void FuncSpacePress() {
		switch (optans) {
		case 0:
			try {
				IISUI.clientSocket = new Socket(IISUI.HOSTNAME, IISUI.PORT_NUMBER);
				IISUI.outpStreSo = IISUI.clientSocket.getOutputStream();
				IISUI.inpuStreSo = IISUI.clientSocket.getInputStream();
				IISUI.LoadInfrastructureData();
				IISUI.sysOut.atAlias.append("\nConnection Established and Infrastructure data is loaded.");
				IISUI.tc.sels.subList(IISUI.tc.ind, IISUI.tc.ind + 2).clear();
				IISUI.tc.ind -= 2;
				IISUI.tc.Set2ndLastSel();
				IISUI.tc.CalcSelectionPos();
			} catch (UnknownHostException e) {
				IISUI.sysOut.atAlias.append("\n" + e.toString());
				e.printStackTrace();
			} catch (IOException e) {
				IISUI.sysOut.atAlias.append("\n" + e.toString());
				e.printStackTrace();
			}
			IISUI.tc.DrawTextAndSelection();
			break;
		}
	}

}
