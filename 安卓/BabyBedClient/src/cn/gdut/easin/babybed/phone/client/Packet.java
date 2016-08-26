package cn.gdut.easin.babybed.phone.client;

import java.io.ByteArrayInputStream;
import java.io.DataInputStream;
import java.io.IOException;

import java.text.SimpleDateFormat;
import java.util.Arrays;
import java.util.Date;

/*对数据打包*/
public class Packet {
	static final int TYPE_PHONE  = 0;
	
	public Packet() {
		// TODO Auto-generated constructor stub
	}
	
	/*
	 * 把MAC地址转换为byte数组
	 * MAC格式： "xx-xx-xx-xx-xx-xx"
	 * */
	public static byte[] macString2Bytes(String mac) {
		String[] split = mac.split("-");
		byte[] bytes = new byte[6];
		for(int i=0; i<split.length; i++) {
			bytes[i] = (byte) Integer.parseInt(split[i], 16);
		}
		
		return bytes;
	}
	
	public static String macBytes2String(byte[] mac) {
		String strmac = "";
		if(mac.length == 6) {
			for(int i=0; i<mac.length; i++) {
				String str = Integer.toHexString(mac[i]);
				if(str.length() > 2)
					str = str.substring(6);
				strmac += (str + "-");
			}
		}

		return strmac.substring(0, strmac.length()-1);
	}
	
	public static byte[] getDrivePacket(String mac) {
		byte[] bytes = Packet.macString2Bytes(mac);

		byte[] temp = new byte[bytes.length + 1];
		for(int i=0; i<bytes.length; i++) {
			temp[i] = bytes[i];
		}
		
		temp[temp.length-1] = Packet.TYPE_PHONE;
		return temp;
	}
	
	public static byte[] getCommandPacket(int cmd) {
		byte[] bytes = new byte[5];
		bytes[0] = (byte)0xFB;
		bytes[1] = (byte)0x8A;
		bytes[2] = (byte)0x01;//length
		bytes[3] = (byte)0x00;
		
		bytes[4] = (byte)cmd;
		return bytes;		
	}
	
	public static BedStatus getBedStatus(byte[] bytes) {
		BedStatus bStatus = new BedStatus();
		
		int rtidx = 4;
		byte[] rtemp = Arrays.copyOfRange(bytes, rtidx, rtidx+8);
		double roomtemperature = byteArrayToDouble(rtemp);
		bStatus.setRoomTemperature(roomtemperature);
		
		int btidx = 12;
		byte[] btemp = Arrays.copyOfRange(bytes, btidx, btidx+8);
		double babytemperature = byteArrayToDouble(btemp);
		bStatus.setBabyTemperature(babytemperature);
		
		int tidx = 20;
		byte[] ttemp = Arrays.copyOfRange(bytes, tidx, tidx+4);
		int time = byteArrayToInt(ttemp);
		bStatus.setTime(time);
		//status
		byte status = bytes[24];
		if((status & 0x01) != 0)
			bStatus.setIsKick(true);
		if((status & 0x02) != 0)
			bStatus.setIsHeating(true);
		if((status & 0x04) != 0)
			bStatus.setIsOnBed(true);
		if((status & 0x08) != 0)
			bStatus.setIsCry(true);
		if((status & 0x10) != 0)
			bStatus.setIsSwing(true);
		if((status & 0x20) != 0)
			bStatus.setIsPee(true);
		if((status & 0x40) != 0)
			bStatus.setIsPlay(true);
		return bStatus;
	}
	
	public static double byteArrayToDouble(byte[] buf) {
		byte[] b = new byte[8];
		int i=8;
		while(i>0) {
			b[i-1] = buf[8-i];
			i--;
		}
		
		ByteArrayInputStream bintput = new ByteArrayInputStream(b);
		DataInputStream dintput = new DataInputStream(bintput);
		double d = 0;
		try {
			d = dintput.readDouble();
		} catch (IOException e) {
			e.printStackTrace();
		}
		return d;
	}
	
	public static int byteArrayToInt(byte[] buf) {
		byte[] b = new byte[4];
		int i=4;
		while(i>0) {
			b[i-1] = buf[4-i];
			i--;
		}
		
		ByteArrayInputStream bintput = new ByteArrayInputStream(b);
		DataInputStream dintput = new DataInputStream(bintput);
		i = 0;
		try {
			i = dintput.readInt();
		} catch (IOException e) {
			e.printStackTrace();
		}
		return i;
	}
	
	 public static String date2TimeStamp(long date,String format){  
	    	SimpleDateFormat sdf = new SimpleDateFormat(format);
	    	String d = sdf.format(new Date(date*1000));
	    	return d;
	  }  
}
