editor = null;

function adjustEditor () {

	var h = $(window).height ()-$('.toolbar').height ();
	$('#editor').height (h);
	if (editor)
		editor.resize();
}

function saveEditor (filename) {

	var postData = 'content='+encodeURIComponent(editor.getValue());

	$.ajax({ 
			type: "POST",
			url: "index.v1?cmd=save&f="+encodeURIComponent(filename),
			/* headers: { 'Connection' : 'close' },  IE7 bug */
			data: postData,
			async: true,
			error : function (xhr, status) {
			
			},
			success: function(data, status, xhr) {

				if (data=="OK") {
					parent.output.location.reload ();
				}
				
			}
		});
}


$(document).ready(function() 
{

		
	$(window).resize(function() 
	{		
		adjustEditor ();
	});
	adjustEditor ();
	
});