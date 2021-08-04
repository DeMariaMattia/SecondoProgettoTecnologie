using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO.Ports;
using System.Linq;
using System.Net;
using System.Net.Mail;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Forms;
using System.Windows.Input;
using System.Windows.Interop;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace MandaEmail
{
    /// <summary>
    /// Logica di interazione per MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        SerialPort serial = new SerialPort();
        string Mittente = "progettoposta2021@gmail.com";
        string Password = "Posta2021";
        string Destinatario = "progettoposta2021@gmail.com";
        string Oggetto = "Email dalla cassetta postale";
        string Testo = "";
        public MainWindow()
        {
            InitializeComponent();
            serial.PortName = "COM3";
            serial.BaudRate = 9600;
            serial.Open();
            serial.DataReceived += COM_DataRecived;
           
        }
        private void COM_DataRecived(object sender, SerialDataReceivedEventArgs e)
        {

            //cose da fare se arriva qualcosa via seriale

            char carattereLetto = (char)serial.ReadChar();
           if(carattereLetto=='a')
           {
                Testo = "La cassetta ha rilevato un movimento,controlla , è stato inserito qualcosa al suo interno!!";
                Mail(Mittente, Destinatario, Oggetto, Testo, Password);
                Testo = "";
            }
           else if (carattereLetto == 'b')
           {
                Testo = "La cassetta ha rilevato un tentativo di accesso da una chiave non registrata ,controlla e verifica che sia tutto nella norma!!";
                Mail(Mittente, Destinatario, Oggetto, Testo, Password);
                Testo = "";
            }
            else if (carattereLetto == 'c')
            {
                Testo = "La lettera è stata prelevata con successo!!";
                Mail(Mittente, Destinatario, Oggetto, Testo, Password);
                Testo = "";
            }

        }



            static bool Mail(string mittente, string destinatario, string oggetto, string testo, string password)//metodo per inviare la mail
        {
            try
            {
                //creo un oggetto di tipo networkCredential con le credenziali (e-mail, password) dell'utente
                NetworkCredential credenzialiAccount = new NetworkCredential(mittente, password);

                //creo la mail
                MailMessage mail = new MailMessage()
                {
                    From = new MailAddress(mittente),
                    Subject = oggetto,
                    Body = testo
                };

                mail.To.Add(new MailAddress(destinatario));

                // Smtp client
                SmtpClient client = new SmtpClient()
                {
                    Port = 587,
                    DeliveryMethod = SmtpDeliveryMethod.Network,
                    UseDefaultCredentials = false,
                    Host = "smtp.gmail.com",
                    EnableSsl = true,//decido se cifrare o meno il messaggio
                    Credentials = credenzialiAccount
                };

                //invio la mail
                client.Send(mail);
            }
            catch (Exception ex)
            {
                return false;
            }

            return true;
            //Console.ReadKey();
        }

        
    }
}
