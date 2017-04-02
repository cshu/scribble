package griddoor.logging;

import java.util.logging.Logger;

public class FileLogger extends Logger {

	protected FileLogger(String name, String resourceBundleName) {
		super(name, resourceBundleName);
	}
	
	public static FileLogger makeFileLogger(){
		return null;
	}

}
