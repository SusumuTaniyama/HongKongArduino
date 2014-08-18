using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WinHongKongFCArduino
{
    public partial class WinHongKongFCArduino : Form
    {
        public WinHongKongFCArduino()
        {
            InitializeComponent();
        }

        private void WinHongKongFCArduino_Load(object sender, EventArgs e)
        {
            string[] portNames = SerialPort.GetPortNames();

            foreach (string portName in portNames)
            {
                cmbCOMPort.Items.Add(portName);
            }

            cmbCOMPort.SelectedIndex = cmbCOMPort.Items.Count - 1;
        }

        private void btnExtract_Click(object sender, EventArgs e)
        {
            // ファイルダイアログ
            SaveFileDialog sfd = new SaveFileDialog();
            sfd.Filter = "FC ROMファイル(*.nes)|*.nes|すべてのファイル(*.*)|*.*";
            if (sfd.ShowDialog() != DialogResult.OK)
            {
                return;
            }


            byte[] header = { 0x4E, 0x45, 0x53, 0x1A, 0x02, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
            byte[] data = new byte[0x8000 + 0x2000 + header.Length];

            header.CopyTo(data, 0);

            // PRG-ROM
            sendControl(13); // CPU R/W(1) + !ROMSEL(0) + M2(4) + PPU /RD(8)
            readROM(data, header.Length, 0x0000, 0x8000);

            // CHR-ROM
            sendControl(2); // CPU R/W(0) + !ROMSEL(1) + M2(0) + PPU /RD(0)
            readROM(data, 0x8000 + header.Length, 0x0000, 0x2000);

            // ファイル書き込み
            File.WriteAllBytes(sfd.FileName, data);
        }

        private void readROM(byte[] data, int offset, int address, int size, bool isLoROM = false)
        {
            string portName = cmbCOMPort.SelectedItem.ToString();
            SerialPort port = new SerialPort(portName, 115200);

            port.Open();

            byte[] cmd = new byte[7];
            if (isLoROM)
            {
                cmd[0] = (byte)'r'; // LoROM
            }
            else
            {
                cmd[0] = (byte)'R'; // HiROM
            }

            cmd[1] = (byte)(address >> (8 * 0));
            cmd[2] = (byte)(address >> (8 * 1));
            cmd[3] = (byte)(address >> (8 * 2));

            cmd[4] = (byte)(size >> (8 * 0));
            cmd[5] = (byte)(size >> (8 * 1));
            cmd[6] = (byte)(size >> (8 * 2));

            port.Write(cmd, 0, cmd.Length);

            int readbyte = 0;
            while (readbyte < size)
            {
                readbyte += port.Read(data, offset + readbyte, size - readbyte);

                // 進行状況表示
                //toolStripProgressBar.Value = readbyte;
                Application.DoEvents();

                System.Threading.Thread.Sleep(1);
            }

            port.Close();
            port.Dispose();
        }

        private void sendControl(byte b)
        {
            string portName = cmbCOMPort.SelectedItem.ToString();
            SerialPort port = new SerialPort(portName, 115200);

            port.Open();

            byte[] cmd = new byte[2];
            cmd[0] = (byte)'c';
            cmd[1] = b;
            port.Write(cmd, 0, cmd.Length);

            port.Close();
            port.Dispose();
        }
    }
}
