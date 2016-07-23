#ifndef REQUESTLISTENER_H
#define REQUESTLISTENER_H

#include <UI/DynamicTreeWidgetItem.h>
#include <Data/TransferRequest.h>
#include <QThread>
#include <QTcpServer>
#include <memory>
#include <functional>

class TransferListener;
class MainWindow;

// The main transfer listener - identifies incoming acknowledges
// according to the list of local pending transfers and start the
// real transfer from this endpoint to the remote one
class ListenerSocketWrapper : public QObject {
  Q_OBJECT
public:
  ListenerSocketWrapper(TransferListener& parent);

private:
  friend class TransferListener;
  TransferListener& m_parent;

  QTcpServer m_server;

private slots:
  void new_transfer_connection();
  void socket_ready_read();
  void socket_error(QAbstractSocket::SocketError err);
};

class TransferListener : public QThread {
  Q_OBJECT

  friend class ListenerSocketWrapper;
public:

  TransferListener(MainWindow *main_win, std::function<bool(TransferRequest&, DynamicTreeWidgetItem*&)> trans_retriever);

  void set_transfer_port(int local_transfer_port);

private:
  void run() Q_DECL_OVERRIDE;

  MainWindow *m_main_win = nullptr;
  std::function<bool(TransferRequest&, DynamicTreeWidgetItem*&)> m_trans_retriever; // Needs to be thread-safe

  int m_local_transfer_port = 67;
};

#endif // REQUESTLISTENER_H
