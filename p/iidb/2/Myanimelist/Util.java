package griddoor.Myanimelist;

import java.awt.HeadlessException;
import java.awt.datatransfer.UnsupportedFlavorException;
import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.net.URLEncoder;

import org.jsoup.Jsoup;

import griddoor.IIPropSets.MalAnime;
import griddoor.util.EnvironmentUtil;

public class Util {

	public static String getSearchUrlStr(String criterion) throws UnsupportedEncodingException {
		return "http://myanimelist.net/api/anime/search.xml?q=" + URLEncoder.encode(criterion, "UTF-8");
	}

	public static MalAnime[] parseClipboardForMalAnimes() throws HeadlessException, UnsupportedFlavorException, IOException {
		org.jsoup.nodes.Document doc = Jsoup.parse(EnvironmentUtil.getStringFromSystemClipboard());
		org.jsoup.select.Elements entries = doc.getElementsByTag("entry");
		MalAnime[] valtoret = new MalAnime[entries.size()];
		int count = 0;
		for (org.jsoup.nodes.Element entry : entries) {
			valtoret[count] = new MalAnime();
			for (org.jsoup.nodes.Element field : entry.children()) {
				switch (field.tagName()) {
				case "id":
					valtoret[count].id = field.ownText().trim();
					break;
				case "title":
					valtoret[count].title = field.ownText().trim();
					break;
				case "english":
					valtoret[count].english = field.ownText().trim();
					break;
				case "synonyms":
					valtoret[count].synonyms = field.ownText().trim();
					break;
				case "episodes":
					valtoret[count].episodes = field.ownText().trim();
					break;
				case "score":
					valtoret[count].score = field.ownText().trim();
					break;
				case "type":
					valtoret[count].type = field.ownText().trim();
					break;
				case "status":
					valtoret[count].status = field.ownText().trim();
					break;
				case "start_date":
					valtoret[count].start_date = field.ownText().trim();
					break;
				case "end_date":
					valtoret[count].end_date = field.ownText().trim();
					break;
				case "synopsis":
					valtoret[count].synopsis = field.ownText().trim();
					break;
				case "image":
					valtoret[count].image = field.ownText().trim();
					break;
				}
			}
			count++;
		}
		return valtoret;
	}
}
