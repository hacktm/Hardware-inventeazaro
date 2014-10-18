var Config = new Object(
  {
    apiUrl  : 'http://medical.devicehub.net:8000',
    version : '0.0.1'


  }


)

function LoginController()
{
$('input[name="submit"]').click(function (e) {
    e.preventDefault();
    var self = this;
    var loginData = $(self).parents("form").serialize();
    $.ajax(
      {
        type: 'POST',
        url: Config.apiUrl + '/auth',
        data : loginData,


      }
    )
    .fail(function(response){
        $('input[name="username"]').css('background-color', "#df9c9c");
        $('input[name="password"]').css('background-color', "#df9c9c");
        if(response.responseJSON.non_field_errors == undefined)
            response.responseJSON.non_field_errors = ['Internal error'];
        $('form').append('<p style="color:#f57878">' + response.responseJSON.non_field_errors[0] + '</p>' );
    })
    .done(function(response){
        sessionStorage.token = response.token;


        window.location = 'profile.html'

    });


})
}
function IndexController()
{
  $('.navbar').removeClass('navbar-inverse');


}
function ProfileController()
{
  $.ajax(
    {
      type: 'GET',
      url: Config.apiUrl + '/profile'
    }
  )
  .fail(function(response){

    if(response.statusText=='UNAUTHORIZED'){
      window.location = 'login.html';
    console.log(response);
    }
  })
  .done(function(response){

      $('[name="name"]').html(response.first_name + ' ' + response.last_name);
      $('[name="email"]').html(response.email);
      for(key in response.userprofile){
        val = response.userprofile[key];
        $('[name="'+ key +'"]').html(val);

      }
      $('[name]').editable(
        {
          type: 'text',
          url: Config.apiUrl + '/profile',
          pk: 1,
          placement: 'top',
          display: function(value, response) {
              //render response into element
              $(this).html(response);
          }

        }
      );

  });
}
(function ($){
$.fn.editable.defaults.mode = 'inline';

  var page =  window.location.pathname.replace('/Hardware-inventeazaro/', '').replace('/', '').replace('.html', '');
  if(page == '')
    page = 'index'
  page = page.charAt(0).toUpperCase() + page.slice(1);
  var fn = window[page + 'Controller'];
  if(typeof fn === 'function') {
      if(page != 'auth' && page != 'index')
        {
        $.ajaxSetup({
        beforeSend: function (request)      {
          request.setRequestHeader("Authorization", 'Token ' + sessionStorage.token || "None");

        }
        });
        }
      fn();
  }

})($);
