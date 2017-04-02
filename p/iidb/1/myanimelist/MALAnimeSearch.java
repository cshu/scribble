package griddoor.myanimelist;

import griddoor.iiswingui.QueMALEntry;

import java.awt.HeadlessException;
import java.awt.Toolkit;
import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.UnsupportedFlavorException;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.UnsupportedEncodingException;
import java.net.URI;
import java.net.URL;
import java.net.URLEncoder;
import java.util.ArrayList;

import org.jsoup.Jsoup;

//todo: QueMALEntry is used in this class, it should be removed
public class MALAnimeSearch {

	public static String usernamePasswd;
	public String criterion;
	public StringBuilder strBuilder = new StringBuilder();
	public org.jsoup.nodes.Document doc;
	
	public static String getSearchUrlStr(String criterion) throws UnsupportedEncodingException{
		return "http://myanimelist.net/api/anime/search.xml?q=" + URLEncoder.encode(criterion, "UTF-8");
	}
	
	public MALAnimeSearch() throws HeadlessException, UnsupportedFlavorException, IOException {
		doc = Jsoup.parse( (String) Toolkit.getDefaultToolkit().getSystemClipboard().getData(DataFlavor.stringFlavor));
	}
	
	public MALAnimeSearch(String criterion) throws IOException {
		this.criterion = criterion;
		URL url = new URL(getSearchUrlStr(criterion));

		java.net.URLConnection connection = url.openConnection();
		String basicAuth = "Basic " + javax.xml.bind.DatatypeConverter.printBase64Binary(usernamePasswd.getBytes());
		connection.setRequestProperty("Authorization", basicAuth);

		BufferedReader in = new BufferedReader(new InputStreamReader(connection.getInputStream()));

		String inputLine;
		while ((inputLine = in.readLine()) != null)
			strBuilder.append(inputLine);
		in.close();

		doc = Jsoup.parse(strBuilder.toString());
	}

	public void fillListWithEntries(ArrayList<QueMALEntry> queMals) {
		org.jsoup.select.Elements entries = doc.getElementsByTag("entry");
		for (org.jsoup.nodes.Element entry : entries) {
			long id = 0;
			String title = null;
			String english = null;
			String synonyms = null;
			long episodes = 0;
			String score = null;
			String type = null;
			String status = null;
			String start_date = null;
			String end_date = null;
			String synopsis = null;
			String image = null;
			for (org.jsoup.nodes.Element field : entry.children()) {
				switch (field.tagName()) {
				case "id":
					id = Long.parseLong(field.ownText().trim());
					break;
				case "title":
					title = field.ownText().trim();
					break;
				case "english":
					english = field.ownText().trim();
					break;
				case "synonyms":
					synonyms = field.ownText().trim();
					break;
				case "episodes":
					episodes = Long.parseLong(field.ownText().trim());
					break;
				case "score":
					score = field.ownText().trim();
					break;
				case "type":
					type = field.ownText().trim();
					break;
				case "status":
					status = field.ownText().trim();
					break;
				case "start_date":
					start_date = field.ownText().trim();
					break;
				case "end_date":
					end_date = field.ownText().trim();
					break;
				case "synopsis":
					synopsis = field.ownText().trim();
					break;
				case "image":
					image = field.ownText().trim();
					break;
				}
			}
			QueMALEntry q = new QueMALEntry(id, title, english, synonyms, episodes, score, type);
			q.status = status;
			q.start_date = start_date;
			q.end_date = end_date;
			q.synopsis = synopsis;
			q.image = image;
			q.xml = entry.outerHtml().replace("\r", "").replace("\n", "");
			queMals.add(q);
		}

	}

}
