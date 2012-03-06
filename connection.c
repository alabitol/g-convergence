/******************************************************************************
 * Authors: Tolu Alabi
 *          Zach Butler
 *          Martin Dluhos
 *
 * Created: March 1, 2012
 * Description: Handle sending a certificate request to the website and
 *              receiving its response.
 ******************************************************************************/

struct connection_info_struct
{
  int connectiontype;
  struct MHD_PostProcessor *postprocessor;
  const char *answerstring;
  int answercode;
};

/* Handles the connection of a client. The address of this function needs to
 * be passed to MHD_start_daemon.
 *
 * NOTE: this was initially static- make sure this works
 */
int
answer_to_connection (void *cls, struct MHD_Connection *connection,
    const char *url, const char *method,
    const char *version, const char *upload_data,
    size_t *upload_data_size, void **con_cls)
{
  struct connection_info_struct *con_info;

  /* We need to create a post processor which retrieves a response to the
   * client. This saves response attributes to con_info
   */
  con_info->postprocessor =
    MHD_create_post_processor (connection, POSTBUFFERSIZE,
	get_response, (void *) con_info);

  /* Retrieve response attributes from con_info to send a response. */
  send_response ();

  return 1;
}

/* Clean up after the request completes closing the connection.
 */
void
request_completed (void *cls, struct MHD_Connection *connection,
    void **con_cls, enum MHD_RequestTerminationCode toe)
{

}
