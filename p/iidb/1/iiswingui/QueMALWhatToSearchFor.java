package griddoor.iiswingui;

import griddoor.myanimelist.MALAnimeSearch;

import java.awt.Desktop;
import java.awt.HeadlessException;
import java.awt.datatransfer.UnsupportedFlavorException;
import java.awt.event.KeyEvent;
import java.io.IOException;
import java.net.URISyntaxException;

import javax.swing.JOptionPane;
import javax.xml.parsers.ParserConfigurationException;

import org.xml.sax.SAXException;

public class QueMALWhatToSearchFor extends QueToAnsWithStr {

	public QueMALWhatToSearchFor() {
		super("What to search for?");
	}

	@Override
	public void FuncKeyPressed(KeyEvent e) {
		if (e.getKeyCode() == KeyEvent.VK_SPACE) {
			if (IISUI.tcMal.queMals.size() > 0)
				if (JOptionPane.showConfirmDialog(IISUI.theJfr, "Show the MAL Console? Or cancel to do a new search.", "There are still animes listed in console", JOptionPane.OK_CANCEL_OPTION) == JOptionPane.OK_OPTION) {
					IISUI.tc = IISUI.tcMal;
					IISUI.tc.CalcSelectionPos();
					IISUI.tc.DrawTextAndSelection();
					return;
				}
			if(strans==null)
				return;
			MALAnimeSearch ap = null;
			try {
				if (JOptionPane.showConfirmDialog(IISUI.theJfr, "Use Clipboard as IPC with Browser? Or cancel to send request.", "Search Methods", JOptionPane.OK_CANCEL_OPTION) == JOptionPane.OK_OPTION) {
					Desktop.getDesktop().browse(new java.net.URI(MALAnimeSearch.getSearchUrlStr(strans)));
					if (JOptionPane.showConfirmDialog(IISUI.theJfr, "Proceed to parse?", "Waiting for data", JOptionPane.OK_CANCEL_OPTION) != JOptionPane.OK_OPTION)
						return;
					ap = new MALAnimeSearch();
					ap.criterion = strans;
				} else
					ap = new MALAnimeSearch(strans);
			} catch (HeadlessException | UnsupportedFlavorException | IOException | URISyntaxException e1) {
				e1.printStackTrace();
				IISUI.sysOut.atAlias.append('\n' + e1.toString());
			}

			IISUI.tc = IISUI.tcMal;
			IISUI.tcMal.title = "MAL MODE   " + ap.criterion;
			IISUI.tcMal.setMalEntriesAndRefr(ap);
		}
	}

}
