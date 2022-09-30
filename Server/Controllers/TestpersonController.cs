using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.Rendering;
using Microsoft.EntityFrameworkCore;
using UIAutomate.Models;

namespace Server.Controllers
{
    public class TestpersonController : Controller
    {
        private readonly TestpersonContext _context;

        public TestpersonController(TestpersonContext context)
        {
            _context = context;
        }

        // GET: Testperson
        public async Task<IActionResult> Index()
        {
              return View(await _context.Testpersons.ToListAsync());
        }

        public async Task<IActionResult> Introduction()
        {
            return View(await _context.Testpersons.ToListAsync());
            //   return _context.SUSTest != null ? 
            //               View(await _context.SUSTest.ToListAsync()) :
            //               Problem("Entity set 'SUSTestContext.SUSTest'  is null.");
        }

        // GET: SUSTest/Edit/5    duplicatet
        public async Task<IActionResult> SUSResults()
        {
            // if (id == null || _context.SUSTest == null)
            // {
            //     return NotFound();
            // }

            IEnumerable<SUSTest> sUSTest = _context.SUSTests.FromSqlRaw<SUSTest>(@"SELECT * FROM SUSTests WHERE TestpersonId == 1;").ToList<SUSTest>();
            if (sUSTest == null)
            {
                return NotFound();
            }
            return View(sUSTest);
        }

        public IEnumerable<SUSTest> DataSeeder(){
                 var cmd = _context.SUSTests.FromSqlRaw<SUSTest>(@"SELECT * FROM SUSTests WHERE TestpersonId == 1;").ToList<SUSTest>();
            return cmd as IEnumerable<SUSTest>;
        }

        public async Task<IActionResult> VisAWIFormular()
        {
            IEnumerable<VisAWITest> vis = _context.VisAWITests.FromSqlRaw<VisAWITest>(@"SELECT * FROM VisAWITests WHERE TestpersonId == 1;").ToList<VisAWITest>();
            if (vis == null)
            {
                return NotFound();
            }
            return View(vis);
        }

        
        public async Task<IActionResult> UEQFormular()
        {
            IEnumerable<UEQTest> vis = _context.UEQTests.FromSqlRaw<UEQTest>(@"SELECT * FROM UEQTests WHERE TestpersonId == 1;").ToList<UEQTest>();
            if (vis == null)
            {
                return NotFound();
            }
            return View(vis);
        }

   // GET: SUSTest/Edit/5
        [HttpPost]
        [ValidateAntiForgeryToken]
        public async Task<IActionResult> createVisAWIs(IFormCollection form)
        {
            if (form == null || _context.Testpersons == null)
            {
                return NotFound();
            }
            // var data = new List<SUSTest>();
            var temp_Ids = form["item.id"];
            var temp_Questions = form["item.Question"];
            var temp_DisagreeEtAll = form["item.DisagreeEtAll"];
            var temp_Disagree = form["item.Disagree"];
            var temp_DisagreeRather = form["item.DisagreeRather"];
            var temp_Neutrals = form["item.Neutral"];
            var temp_AgreeRather = form["item.AgreeRather"];
            var temp_Agree = form["item.Agree"];
            var temp_AgreeEtAll = form["item.AgreeEtAll"];

            for (int i = 0; i < form["item.id"].Count(); i++)
            {
                var erg = _context.Find<VisAWITest>(Convert.ToInt32(temp_Ids[i]));
                erg.Question = temp_Questions[i];
                erg.DisagreeEtAll = ConvertToBool(temp_DisagreeEtAll[i]);
                erg.Disagree = ConvertToBool(temp_Disagree[i]);
                erg.DisagreeRather = ConvertToBool(temp_DisagreeRather[i]);
                erg.Neutral = ConvertToBool(temp_Neutrals[i]);
                erg.AgreeRather = ConvertToBool(temp_AgreeRather[i]);
                erg.Agree = ConvertToBool(temp_Agree[i]);
                erg.AgreeEtAll = ConvertToBool(temp_AgreeEtAll[i]);
                if (ModelState.IsValid)
                {
                    _context.Update(erg);
                    await _context.SaveChangesAsync();
                }
            }
            return View("~/Views/Home/Index.cshtml");
        }


 // GET: SUSTest/Edit/5
        [HttpPost]
        [ValidateAntiForgeryToken]
        public async Task<IActionResult> createUEQs(IFormCollection form)
        {
            if (form == null || _context.Testpersons == null)
            {
                return NotFound();
            }
            // var data = new List<SUSTest>();
            var temp_Ids = form["item.id"];
            var temp_LeftWord = form["item.LeftWord"];
            var temp_RightWord = form["item.RightWord"];
            var temp_one = form["item.one"];
            var temp_two = form["item.two"];
            var temp_three = form["item.three"];
            var temp_four = form["item.four"];
            var temp_five = form["item.five"];
            var temp_six = form["item.six"];
            var temp_seven = form["item.seven"];

            for (int i = 0; i < form["item.id"].Count(); i++)
            {
                var erg = _context.Find<UEQTest>(Convert.ToInt32(temp_Ids[i]));
                erg.LeftWord = temp_LeftWord[i];
                erg.RightWord = temp_RightWord[i];
                erg.one = ConvertToBool(temp_one[i]);
                erg.two = ConvertToBool(temp_two[i]);
                erg.three = ConvertToBool(temp_three[i]);
                erg.four = ConvertToBool(temp_four[i]);
                erg.five = ConvertToBool(temp_five[i]);
                erg.six = ConvertToBool(temp_six[i]);
                erg.seven = ConvertToBool(temp_seven[i]);
                if (ModelState.IsValid)
                {
                    _context.Update(erg);
                    await _context.SaveChangesAsync();
                }
            }
            return View("~/Views/Home/Index.cshtml");
        }

         // GET: SUSTest/Edit/5
        [HttpPost]
        [ValidateAntiForgeryToken]
        public async Task<IActionResult> CreateForm(IFormCollection form)
        {
            if (form == null || _context.Testpersons == null)
            {
                return NotFound();
            }
            var data = new List<SUSTest>();
            var temp_Ids = form["item.id"];
            var temp_Questions = form["item.Question"];
            var temp_SDisagrees = form["item.StronglyDisagree"];
            var temp_Disagrees = form["item.Disagree"];
            var temp_Neutrals = form["item.Neutral"];
            var temp_Agrees = form["item.Agree"];
            var temp_SAgrees = form["item.StronglyAgree"];

            for (int i = 0; i < form["item.id"].Count(); i++)
            {
                var erg = _context.Find<SUSTest>(Convert.ToInt32(temp_Ids[i]));
                erg.Question = temp_Questions[i];
                erg.StronglyDisagree = ConvertToBool(temp_SDisagrees[i]);
                erg.Disagree = ConvertToBool(temp_Disagrees[i]);
                erg.Neutral = ConvertToBool(temp_Neutrals[i]);
                erg.Agree = ConvertToBool(temp_Agrees[i]);
                erg.StronglyAgree = ConvertToBool(temp_SAgrees[i]);
                if (ModelState.IsValid)
                {
                    _context.Update(erg);
                    await _context.SaveChangesAsync();
                }
            }
            return View("~/Views/Home/Index.cshtml");
        }


        private bool ConvertToBool(string temp){
            bool erg = false;
            if (temp.Equals("true"))
            {
                erg = true;
            }else if(temp.Equals("false")){
                erg= false;
            }
            return erg;
        }


        // GET: Testperson/Details/5
        public async Task<IActionResult> Details(int? id)
        {
            if (id == null || _context.Testpersons == null)
            {
                return NotFound();
            }

            var testperson = await _context.Testpersons
                .FirstOrDefaultAsync(m => m.Id == id);
            if (testperson == null)
            {
                return NotFound();
            }

            return View(testperson);
        }

        // GET: Testperson/Create
        public IActionResult Create()
        {
            return View();
        }

        // POST: Testperson/Create
        // To protect from overposting attacks, enable the specific properties you want to bind to.
        // For more details, see http://go.microsoft.com/fwlink/?LinkId=317598.
        [HttpPost]
        [ValidateAntiForgeryToken]
        public async Task<IActionResult> Create([Bind("Id,anonymName")] Testperson testperson)
        {
            if (ModelState.IsValid)
            {
                _context.Add(testperson);
                await _context.SaveChangesAsync();
                return RedirectToAction(nameof(Index));
            }
            return View(testperson);
        }

        // GET: Testperson/Edit/5
        public async Task<IActionResult> Edit(int? id)
        {
            if (id == null || _context.Testpersons == null)
            {
                return NotFound();
            }

            var testperson = await _context.Testpersons.FindAsync(id);
            if (testperson == null)
            {
                return NotFound();
            }
            return View(testperson);
        }

        // POST: Testperson/Edit/5
        // To protect from overposting attacks, enable the specific properties you want to bind to.
        // For more details, see http://go.microsoft.com/fwlink/?LinkId=317598.
        [HttpPost]
        [ValidateAntiForgeryToken]
        public async Task<IActionResult> Edit(int id, [Bind("Id,anonymName")] Testperson testperson)
        {
            if (id != testperson.Id)
            {
                return NotFound();
            }

            if (ModelState.IsValid)
            {
                try
                {
                    _context.Update(testperson);
                    await _context.SaveChangesAsync();
                }
                catch (DbUpdateConcurrencyException)
                {
                    if (!TestpersonExists(testperson.Id))
                    {
                        return NotFound();
                    }
                    else
                    {
                        throw;
                    }
                }
                return RedirectToAction(nameof(Index));
            }
            return View(testperson);
        }

        // GET: Testperson/Delete/5
        public async Task<IActionResult> Delete(int? id)
        {
            if (id == null || _context.Testpersons == null)
            {
                return NotFound();
            }

            var testperson = await _context.Testpersons
                .FirstOrDefaultAsync(m => m.Id == id);
            if (testperson == null)
            {
                return NotFound();
            }

            return View(testperson);
        }

        // POST: Testperson/Delete/5
        [HttpPost, ActionName("Delete")]
        [ValidateAntiForgeryToken]
        public async Task<IActionResult> DeleteConfirmed(int id)
        {
            if (_context.Testpersons == null)
            {
                return Problem("Entity set 'TestpersonContext.Testpersons'  is null.");
            }
            var testperson = await _context.Testpersons.FindAsync(id);
            if (testperson != null)
            {
                _context.Testpersons.Remove(testperson);
            }
            
            await _context.SaveChangesAsync();
            return RedirectToAction(nameof(Index));
        }

        private bool TestpersonExists(int id)
        {
          return _context.Testpersons.Any(e => e.Id == id);
        }
    }
}
