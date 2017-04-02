package griddoor.IIPropSets;

import griddoor.iiSwBasedSpInterfaces.Globals;

import java.io.File;
import java.text.DecimalFormat;
import java.text.SimpleDateFormat;
import java.util.Date;

public class Formatters {
	public static StringBuilder formatAsShortNoteSynopsis(SingleInstanceSense sns){
		StringBuilder sb = new StringBuilder();
		for(Long sisId : sns.lons)
			sb.append(String.valueOf(SingleInstanceSense.allsenses.get(sisId)));
		return sb.append(sns.gets(Globals.SHORTNOTE_text));
	}
	public static StringBuilder formatAsFileStorage(SingleInstanceSense fil){
		DecimalFormat formatter = new DecimalFormat("#.0");
		SimpleDateFormat sdf = new SimpleDateFormat("yyyyMMdd HHmmss");
		StringBuilder sb = new StringBuilder(String.format("%7s", formatter.format((double)fil.getl(Globals.FILE_size)/1048576)));
		sb.append(" MB  ");
		sb.append(sdf.format(new Date(fil.getl(Globals.FILE_wtime))));
		sb.append("  ");
		sb.append(String.format("%10s", fil.gets(Globals.FILE_snm)));
		sb.append("  ");
		sb.append(fil.gets(Globals.FILE_onm));
		return sb;
	}
	public static StringBuilder format(File f){
		DecimalFormat formatter = new DecimalFormat("#.0");
		SimpleDateFormat sdf = new SimpleDateFormat("yyyyMMdd HHmmss");
		StringBuilder sb = new StringBuilder(String.format("%7s", formatter.format((double)f.length()/1048576)));
		sb.append(" MB  ");
		sb.append(sdf.format(new Date(f.lastModified())));
		sb.append("  ");
		sb.append(f.getName());
		return sb;
	}
}
