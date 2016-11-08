

class SPI_Interface 
{
	public:
		SPI_Interface(int channel, int speed);
		~SPI_Interface();
	
	private:
		int m_speed;
		int m_channel;
};