package griddoor.iiSwBasedSpInterfaces;

import java.io.IOException;
import java.util.Iterator;
import java.util.logging.FileHandler;
import java.util.logging.Logger;
import java.util.logging.SimpleFormatter;

class LazyLogger implements Iterator<Logger>{//undone what about default methods? might affect implementing Iterator?
	@Override
	public boolean hasNext() {
		return false;
	}
	@Override
	public Logger next() {
		if(logger!=null)
			return logger;
		FileHandler fh;//undone close it necessary???
		try {
			fh = new FileHandler(Globals.exLogFullname,true);//undone use config file
		} catch (SecurityException | IOException e) {
			e.printStackTrace();
			return null;
		}
		logger = Logger.getLogger("TheLgrNm");
		logger.addHandler(fh);
		fh.setFormatter(new SimpleFormatter());//todo use utc time formatter maybe there is ?
		//logger.setUseParentHandlers(false);//debug you need to use this line for disabling consolehandler?!
		logger.info("The global logger gets initialized and ready.");
		return logger;
	}
	static Logger logger;
}
