namespace WinHongKongFCArduino
{
    partial class WinHongKongFCArduino
    {
        /// <summary>
        /// 必要なデザイナー変数です。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 使用中のリソースをすべてクリーンアップします。
        /// </summary>
        /// <param name="disposing">マネージ リソースが破棄される場合 true、破棄されない場合は false です。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows フォーム デザイナーで生成されたコード

        /// <summary>
        /// デザイナー サポートに必要なメソッドです。このメソッドの内容を
        /// コード エディターで変更しないでください。
        /// </summary>
        private void InitializeComponent()
        {
            this.btnExtract = new System.Windows.Forms.Button();
            this.label5 = new System.Windows.Forms.Label();
            this.cmbCOMPort = new System.Windows.Forms.ComboBox();
            this.SuspendLayout();
            // 
            // btnExtract
            // 
            this.btnExtract.Location = new System.Drawing.Point(171, 225);
            this.btnExtract.Name = "btnExtract";
            this.btnExtract.Size = new System.Drawing.Size(93, 23);
            this.btnExtract.TabIndex = 0;
            this.btnExtract.Text = "ROM吸い出し";
            this.btnExtract.UseVisualStyleBackColor = true;
            this.btnExtract.Click += new System.EventHandler(this.btnExtract_Click);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(11, 15);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(71, 12);
            this.label5.TabIndex = 14;
            this.label5.Text = "シリアルポート:";
            // 
            // cmbCOMPort
            // 
            this.cmbCOMPort.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbCOMPort.FormattingEnabled = true;
            this.cmbCOMPort.Location = new System.Drawing.Point(84, 12);
            this.cmbCOMPort.Name = "cmbCOMPort";
            this.cmbCOMPort.Size = new System.Drawing.Size(76, 20);
            this.cmbCOMPort.TabIndex = 13;
            // 
            // WinHongKongFCArduino
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(284, 262);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.cmbCOMPort);
            this.Controls.Add(this.btnExtract);
            this.Name = "WinHongKongFCArduino";
            this.Text = "WinHongKongFCArduino";
            this.Load += new System.EventHandler(this.WinHongKongFCArduino_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnExtract;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.ComboBox cmbCOMPort;
    }
}

