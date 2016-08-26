package cn.gdut.easin.babybed.phone.client;

public class BedStatus {
	private int userid=-1;
	private double room_temperature = 0;
	private double baby_temperature = 0;
	private boolean isHeating = false;//�Ƿ��ڼ���
	private boolean isOnBed = false;//�Ƿ��ڴ���
	private boolean isCry = false;//�Ƿ����
	private boolean isKick = false;//�Ƿ��߱���
	private boolean isPee = false;//�Ƿ�����
	private boolean isPlay = false; //�Ƿ񲥷�����
	private boolean isSwing = false; //�Ƿ���ҡ��
	private long time = 0; //��¼ʱ��(ʱ���)
	
	public BedStatus() {
		
	}
	
	public void setUserId(int id) {
		this.userid = id;
	}
	
	public void setRoomTemperature(double temperature) {
		this.room_temperature = temperature;
	}
	
	public void setBabyTemperature(double temperature) {
		this.baby_temperature = temperature;
	}
	
	public void setIsHeating(boolean is) {
		this.isHeating = is;
	}
	
	public void setIsOnBed(boolean is) {
		this.isOnBed = is;
	}
	
	public void setIsCry(boolean is) {
		this.isCry = is;
	}
	
	public void setIsKick(boolean is) {
		this.isKick = is;
	}
	
	public void setIsPee(boolean is) {
		this.isPee = is;
	}
	
	public void setTime(long time) {
		this.time = time;
	}
	
	public void setIsSwing(boolean is) {
		this.isSwing = is;
	}
	
	public void setIsPlay(boolean is) {
		this.isPlay = is;
	}
	
	/**************************************************************/
	public int getUserId() {
		return this.userid;
	}
	
	public double getRoomTemperature() {
		return this.room_temperature;
	}
	
	public double getBabyTemperature() {
		return this.baby_temperature;
	}
	
	public boolean isHeating() {
		return this.isHeating;
	}
	
	public boolean isOnbed() {
		return this.isOnBed;
	}
	
	public boolean isCry() {
		return this.isCry;
	}
	
	public boolean isKick() {
		return this.isKick;
	}
	
	public boolean isPee() {
		return this.isPee;
	}
	
	public long getRecordTime() {
		return this.time;
	}
	
	public boolean isSwing() {
		return this.isSwing;
	}
	
	public boolean isPlay() {
		return this.isPlay;
	}
	
}
