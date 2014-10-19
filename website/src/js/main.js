var Config = new Object(
  {
    apiUrl  : 'http://medical.devicehub.net',
    version : '0.0.1'


  }
)

function DashboardController() {
  setInterval(function ()
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
      $("#gravatar").attr("src", response.userprofile.gravatar_img);
  });

  $.ajax(
    {
      type: 'GET',
      url: Config.apiUrl + '/data-latest'
    }
  )
  .fail(function(response){

    if(response.statusText=='UNAUTHORIZED'){
      window.location = 'login.html';
    console.log(response);
    }
  })
  .done(function(response){

      for(key in response) {
        val = response[key];
        $('[name="'+ key +'"]').html(val.value);

      }
      for(var i = 0; i < $('option').length;i++) {
          var opt = $('option')[i];
          opt.text = opt.value;
      }
  });
  }, 1000);

  $.ajax(
    {
      type: 'GET',
      url: Config.apiUrl + '/data-history/pulse'
    }
  )
  .fail(function(response) {
    console.error('fail at data-history');
  })
  .done(function(response){
    var items = [];
    $.each( response, function( key, val ) {
      items.push([val.timestamp, val.value]);
    });

    var chart1; // globally available
    chart1 = new Highcharts.StockChart({
       chart: {
          renderTo: 'pulse-history'
       },
       rangeSelector: {
          selected: 1,
          enabled: false,
       },
       scrollbar: {
         enabled: false,
       },
       series: [{
          name: 'Pulse',
          data: items // predefined JavaScript array
       }]
    });
  });

  $.ajax(
    {
      type: 'GET',
      url: Config.apiUrl + '/data-learn'
    }
  )
  .fail(function(response){
    if(response.statusText=='UNAUTHORIZED') {
      window.location = 'login.html';
      console.log(response);
    } else {
      alert(response);

    }
  })
  .done(function(response){
      $.each( response, function( key, val ) {
        $('.sg tbody').append('<tr><td class="text-center">1</td><td>'+ response.suggestion + '</td></tr>')
      });
  });
}

function LoginController() {
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

function IndexController() {
  $('.navbar').removeClass('navbar-inverse');


}

function ProfileController() {
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

      $("#gravatar").attr("src", response.userprofile.gravatar_img);

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

$.fn.editable.defaults.mode = 'inline';

(function ($){

  var page =  window.location.pathname.replace('/Hardware-inventeazaro/', '').replace('/', '').replace('.html', '');
  if(page == '')
    page = 'index'
  if(page != 'login' && page != 'index')
  {
    $.ajaxSetup({
    beforeSend: function (request)      {
      request.setRequestHeader("Authorization", 'Token ' + sessionStorage.token || "None");
  }
  });
  }

  page = page.charAt(0).toUpperCase() + page.slice(1);
  var fn = window[page + 'Controller'];
  if(typeof fn === 'function') {
      fn();
  }

})($);
