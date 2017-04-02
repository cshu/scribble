package griddoor.iiswingui;

import griddoor.util.UtilFuncs;

import java.io.File;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.channels.FileChannel;
import java.nio.channels.FileLock;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Iterator;

import javax.swing.JOptionPane;

public class TextConsoleFiles extends TextConsoleQueList {

	public TextConsoleFiles() {
		title = "FILES MODE";// todo this string is actually useless, (never used)
	}

	public ArrayList<QueFile> queFiles;

	@Override
	public ArrayList<? extends Selectable> getQues() {
		return queFiles;
	}

	@Override
	public void FuncEPress() {
		for (QueFile qf : queFiles)
			qf.toggleChecked();
		DrawTextAndSelection();
	}

	@Override
	public void FuncWPress() {
		if (IISUI.tcMal.selectedEntry == null || QueFile.inheritsProduct != 2)
			return;// todo temp code

		if (JOptionPane.showConfirmDialog(IISUI.theJfr, "Proceed?", "Register file(s)", JOptionPane.OK_CANCEL_OPTION) != JOptionPane.OK_OPTION)
			return;

		byte[] animeId = null;
		byte[] digFileId = new byte[8];
		String shortFilename;
		boolean isInSameDrive;
		Iterator<QueFile> iQf = queFiles.iterator();
		while (iQf.hasNext()) {
			QueFile qf = iQf.next();
			if (!qf.checked)
				continue;
			iQf.remove();
			try {
				//todo: windows only check method?
				isInSameDrive = Character.toLowerCase(qf.fil.getCanonicalPath().charAt(0)) == Character.toLowerCase(IISUI.defaultStorageLoca.charAt(0));

				// todo: firstly check the former filenames and dig file sizes to detect any possible duplicate registration
				IISUI.outpStreSo.write(5);
				UtilFuncs.OutputStreamSendLen16AndStrUtf8(IISUI.outpStreSo, "i");
				IISUI.outpStreSo.write(1);// new instance of digital file
				UtilFuncs.InputStreamReadBytesUntil(8, IISUI.inpuStreSo, digFileId, 0);
				UtilFuncs.OutputStreamSendLen16AndLongSignifBytes(IISUI.outpStreSo, qf.fil.length());

				shortFilename = UtilFuncs.convLongToEquiAlphanum(UtilFuncs.GetLongFromLE8Bytes(digFileId, 0));

				IISUI.outpStreSo.write(5);
				UtilFuncs.OutputStreamSendLen16AndStrUtf8(IISUI.outpStreSo, "j");
				IISUI.outpStreSo.write(1);// new instance of copy of digital file
				IISUI.inpuStreSo.read();// read the copy of dig file ID
				IISUI.inpuStreSo.read();
				IISUI.inpuStreSo.read();
				IISUI.inpuStreSo.read();
				IISUI.inpuStreSo.read();
				IISUI.inpuStreSo.read();
				IISUI.inpuStreSo.read();
				IISUI.inpuStreSo.read();
				IISUI.outpStreSo.write(0);
				IISUI.outpStreSo.write(digFileId);
				UtilFuncs.OutputStreamSendLen16AndStrUtf8(IISUI.outpStreSo, shortFilename);
				IISUI.outpStreSo.write(0);
				IISUI.outpStreSo.write(1);// todo: make this directory configurable!
				IISUI.outpStreSo.write(0);
				IISUI.outpStreSo.write(0);
				IISUI.outpStreSo.write(0);
				IISUI.outpStreSo.write(0);
				IISUI.outpStreSo.write(0);
				IISUI.outpStreSo.write(0);
				IISUI.outpStreSo.write(0);
				byte[] timeCreateAccessWrite = UtilFuncs.GetWinFileTime(qf.fil.getCanonicalPath());
				IISUI.outpStreSo.write(8);
				IISUI.outpStreSo.write(0);
				IISUI.outpStreSo.write(timeCreateAccessWrite, 16, 8);

				IISUI.outpStreSo.write(5);
				UtilFuncs.OutputStreamSendLen16AndStrUtf8(IISUI.outpStreSo, "o");
				UtilFuncs.OutputStreamSendLen16AndStrUtf8(IISUI.outpStreSo, qf.fil.getName());
				IISUI.outpStreSo.write(0);
				IISUI.outpStreSo.write(digFileId);

				switch (QueFile.inheritsProduct) {
				case 2:
					if (animeId == null)
						animeId = Utils.GetOrInsertAnimeIdFromMal();
					IISUI.outpStreSo.write(5);
					UtilFuncs.OutputStreamSendLen16AndStrUtf8(IISUI.outpStreSo, "n");
					IISUI.outpStreSo.write(0);
					IISUI.outpStreSo.write(digFileId);
					IISUI.outpStreSo.write(0);
					IISUI.outpStreSo.write(animeId);
					break;
				}

				//				qf.filloc.release();
				//				qf.filloc.channel().close();

				if (isInSameDrive){
					if(!qf.fil.renameTo(new File(IISUI.defaultStorageLoca + shortFilename)))
						IISUI.sysOut.atAlias.append("\nMoving to storage location failed! "+qf.fil.getName()+':'+shortFilename);
					//fixme Handle exceptional cases? duplicate filename?
				}
				else {
					Files.copy(qf.path, Paths.get(IISUI.defaultStorageLoca + shortFilename));
					File dirFileObj = new File(IISUI.registeredFolder + qf.fil.getParentFile().getName());
					if(!dirFileObj.exists())
						dirFileObj.mkdir();//fixme what if it's a file, not a dir?? although file is forbidden under regFolder in the first place
					File newFileObj = new File(IISUI.registeredFolder + qf.fil.getParentFile().getName() + File.separatorChar + qf.fil.getName());
					while (newFileObj.exists())
						newFileObj = new File(newFileObj.getCanonicalPath() + shortFilename);
					qf.fil.renameTo(newFileObj);//fixme check return bool
				}
			} catch (IOException e) {
				e.printStackTrace();
				IISUI.sysOut.atAlias.append("\n" + e.toString());//undone
			}
		}

		//QueFile.inheritsProduct = 2;
		//QueFile.updateJFrameTitle("");
		ind = 1;
		RefrPrintedQues();
	}

	public void setFilesAndRefr(File[] items) {
		//		if (queFiles != null)
		//			for (QueFile qf : queFiles) {
		//				try {
		//					qf.filloc.release();
		//					qf.filloc.channel().close();
		//				} catch (IOException e) {
		//					IISUI.sysOut.atAlias.append("\n" + e.toString());
		//					e.printStackTrace();
		//				}
		//			}
		queFiles = new ArrayList<QueFile>(items.length);
		for (File item : items) {
			try {
				queFiles.add(new QueFile(item));
			} catch (IOException e) {
				IISUI.sysOut.atAlias.append("\n" + e.toString());
				e.printStackTrace();
			}
		}

		quesOffset = 0;
		ind = 1;
		RefrPrintedQues();
	}

}
