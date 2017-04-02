package griddoor.Myanimelist;

import griddoor.IIPropSets.MalAnime;

import java.util.Iterator;

//? maybe extends list instead? cuz Elements extends list. but isn't that a bother?
public class MalAnimeIterator implements Iterator<MalAnime> {

	Iterator<org.jsoup.nodes.Element> entriesIterator;
	public MalAnimeIterator(org.jsoup.select.Elements entries) {
		entriesIterator = entries.iterator();
	}

	@Override
	public boolean hasNext() {
		return entriesIterator.hasNext();
	}

	@Override
	public MalAnime next() {
		return null;
	}

}
