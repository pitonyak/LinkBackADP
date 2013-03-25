#ifndef LOGPROCESSINGTHREAD_H
#define LOGPROCESSINGTHREAD_H

#include <QThread>

class LogProcessingThread : public QThread
{
public:
  //**************************************************************************
  /*! \brief Default constructor with no backup set.
   *
   *  \param [in] parent is this object's owner and the destructor will destroys all child objects.
   **************************************************************************/
 explicit LogProcessingThread(QObject *parent = 0);

  //**************************************************************************
  /*! \brief Destructor.  */
  //**************************************************************************
  virtual ~LogProcessingThread() { }

  //**************************************************************************
  /*! \brief Return true if cancel was requested on the backup set. */
  //**************************************************************************
  bool isCancelRequested() const { return m_cancelRequested; }

protected:
  //**************************************************************************
  /*! \brief Start the thread and do the entire process. */
  //**************************************************************************
  virtual void run();

signals:

public slots:
  //**************************************************************************
  /*! \brief Set the "cancel requested" flag so that the thread knows it should stop. */
  //**************************************************************************
  void requestCancel();

private:
  //**************************************************************************
  /*! \brief Set when the thread should stop running. */
  //**************************************************************************
  bool m_cancelRequested;
};

#endif // LOGPROCESSINGTHREAD_H
